/*
 * System calls
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/stringify.h>
#include <linux/uaccess.h>

#define RINA_PREFIX "syscalls"

#include "logs.h"
#include "utils.h"
#include "personality.h"
#include "debug.h"
#include "ipcp-utils.h"
#include "du.h"

#define CALL_PERSONALITY(RETVAL, PERS, HOOK, ARGS...)                   \
        do {                                                            \
                LOG_DBG("Handling personality hook %s",                 \
                        __stringify(HOOK));                             \
                                                                        \
                if (PERS == NULL) {                                     \
                        LOG_ERR("No personality registered");           \
                        return -1;                                      \
                }                                                       \
                                                                        \
                ASSERT(PERS);                                           \
                ASSERT(PERS -> ops);                                    \
                                                                        \
                if (PERS -> ops -> HOOK == NULL) {                      \
                        LOG_ERR("Personality has no %s hook",           \
                                __stringify(HOOK));                     \
                        return -1;                                      \
                }                                                       \
                                                                        \
                LOG_DBG("Calling personality hook %s",                  \
                        __stringify(HOOK));                             \
                                                                        \
                RETVAL = PERS -> ops -> HOOK (PERS->data, ##ARGS);      \
        } while (0)

#define CALL_DEFAULT_PERSONALITY(RETVAL, HOOK, ARGS...)                 \
        CALL_PERSONALITY(RETVAL, default_personality, HOOK, ##ARGS)

#ifdef CONFIG_RINA_SYSCALLS_DEBUG
#define SYSCALL_DUMP_ENTER LOG_DBG("Entered %s syscall body", __FUNCTION__)
#define SYSCALL_DUMP_EXIT  LOG_DBG("Exiting %s syscall body", __FUNCTION__)
#else
#define SYSCALL_DUMP_ENTER do { } while (0)
#define SYSCALL_DUMP_EXIT  do { } while (0)
#endif

/*
 * FIXME: Once accepted (...), move them to cond_syscall()
 */

SYSCALL_DEFINE6(ipc_create,
                const char __user *, process_name,
                const char __user *, process_instance,
                const char __user *, entity_name,
                const char __user *, entity_instance,
                ipc_process_id_t,    id,
                const char __user *, type)
{
#ifndef CONFIG_RINA
        (void) process_name;
        (void) process_instance;
        (void) entity_name;
        (void) entity_instance;
        (void) id;
        (void) type;

        return -ENOSYS;
#else
        long          retval;

        struct name * tn;
        char *        tpn;
        char *        tpi;
        char *        ten;
        char *        tei;
        char *        tt;

        SYSCALL_DUMP_ENTER;

        tn = name_create();
        if (!tn) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        /* FIXME: Fix this crappiness */
        tpn = string_from_user(process_name);
        tpi = string_from_user(process_instance);
        ten = string_from_user(entity_name);
        tei = string_from_user(entity_instance);
        if (!name_init_with(tn, tpn, tpi, ten, tei)) {
                SYSCALL_DUMP_EXIT;
                name_destroy(tn);
                return -EFAULT;
        }

        /* Ownership taken, name_destroy() will be enough from now on */

        tt = strdup_from_user(type);
        if (!tt) {
                SYSCALL_DUMP_EXIT;
                name_destroy(tn);
                return -EFAULT;
        }

        LOG_DBG("Parms:");
        LOG_DBG("  Process name     = %s", tn->process_name);
        LOG_DBG("  Process instance = %s", tn->process_instance);
        LOG_DBG("  Entity name      = %s", tn->entity_name);
        LOG_DBG("  Entity instance  = %s", tn->entity_instance);
        LOG_DBG("  Type             = %s", tt);

        CALL_DEFAULT_PERSONALITY(retval, ipc_create, tn, id, tt);

        name_destroy(tn);
        rkfree(tt);

        SYSCALL_DUMP_EXIT;

        return retval;
#endif
}

SYSCALL_DEFINE1(ipc_destroy,
                ipc_process_id_t, id)
{
#ifndef CONFIG_RINA
        (void) id;

        return -ENOSYS;
#else
        long retval;

        SYSCALL_DUMP_ENTER;

        CALL_DEFAULT_PERSONALITY(retval, ipc_destroy, id);

        SYSCALL_DUMP_EXIT;

        return retval;
#endif
}

SYSCALL_DEFINE3(sdu_read,
                port_id_t,     id,
                void __user *, buffer,
                size_t,        size)
{
#ifndef CONFIG_RINA
        (void) id;
        (void) buffer;
        (void) size;

        return -ENOSYS;
#else
        ssize_t      retval;
        struct sdu * tmp;
        size_t       retsize;

        SYSCALL_DUMP_ENTER;

        tmp = NULL;

        CALL_DEFAULT_PERSONALITY(retval, sdu_read, id, &tmp);
        /* Taking ownership from the internal layers */

        LOG_DBG("Personality returned value %zd", retval);

        if (retval) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        if (!sdu_is_ok(tmp)) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        /* NOTE: We don't handle partial copies */
        if (buffer_length(tmp->buffer) > size) {
                SYSCALL_DUMP_EXIT;

                LOG_ERR("Partial copies not handled. SDU size: %zd, "
                        "User space buffer size: %zd",
                        buffer_length(tmp->buffer), size);
                sdu_destroy(tmp);
                return -EFAULT;
        }

        if (copy_to_user(buffer,
                         buffer_data_ro(tmp->buffer),
                         buffer_length(tmp->buffer))) {
                SYSCALL_DUMP_EXIT;

                LOG_ERR("Error copying data to user-space");
                sdu_destroy(tmp);
                return -EFAULT;
        }

        retsize = buffer_length(tmp->buffer);
        sdu_destroy(tmp);

        SYSCALL_DUMP_EXIT;

        return retsize;
#endif
}

SYSCALL_DEFINE3(sdu_write,
                port_id_t,           id,
                const void __user *, buffer,
                size_t,              size)
{
#ifndef CONFIG_RINA
        (void) buffer;
        (void) size;

        return -ENOSYS;
#else
        ssize_t         retval;
        struct sdu *    sdu;
        struct buffer * tmp_buffer;

        SYSCALL_DUMP_ENTER;

        if (!buffer || !size) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        LOG_DBG("Syscall write SDU (size = %zd, port-id = %d)", size, id);

        tmp_buffer = buffer_create(size);
        if (!tmp_buffer) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        ASSERT(buffer_is_ok(tmp_buffer));
        ASSERT(buffer_data_rw(tmp_buffer));

        /* NOTE: We don't handle partial copies */
        if (copy_from_user(buffer_data_rw(tmp_buffer), buffer, size)) {
                SYSCALL_DUMP_EXIT;
                buffer_destroy(tmp_buffer);
                return -EFAULT;
        }

        /* NOTE: sdu_create takes the ownership of the buffer */
        sdu = sdu_create_buffer_with(tmp_buffer);
        if (!sdu) {
                SYSCALL_DUMP_EXIT;
                buffer_destroy(tmp_buffer);
                return -EFAULT;
        }
        ASSERT(sdu_is_ok(sdu));

        /* Passing ownership to the internal layers */
        CALL_DEFAULT_PERSONALITY(retval, sdu_write, id, sdu);
        if (retval) {
                SYSCALL_DUMP_EXIT;
                /* NOTE: Do not destroy SDU, ownership isn't our anymore */
                return -EFAULT;
        }

        SYSCALL_DUMP_EXIT;

        return size;
#endif
}

SYSCALL_DEFINE3(allocate_port,
                ipc_process_id_t, id,
                const char __user *, process_name,
                const char __user *, process_instance)
{
#ifndef CONFIG_RINA
        (void) id;
        (void) process_name;
        (void) process_instance;

        return -ENOSYS;
#else
        port_id_t     retval;
        struct name * tname;

        SYSCALL_DUMP_ENTER;

        tname = name_create();
        if (!tname) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        if (!name_init_with(tname,
                            string_from_user(process_name),
                            string_from_user(process_instance),
                            NULL,
                            NULL)) {
                SYSCALL_DUMP_EXIT;
                name_destroy(tname);
                return -EFAULT;
        }

        CALL_DEFAULT_PERSONALITY(retval, allocate_port, id, tname);

        SYSCALL_DUMP_EXIT;

        return retval;
#endif
}

SYSCALL_DEFINE1(deallocate_port,
                port_id_t, id)
{
#ifndef CONFIG_RINA
        (void) id;

        return -ENOSYS;
#else
        int retval;

        SYSCALL_DUMP_ENTER;

        CALL_DEFAULT_PERSONALITY(retval, deallocate_port, id);

        SYSCALL_DUMP_EXIT;

        return retval;
#endif
}

SYSCALL_DEFINE4(management_sdu_read,
                ipc_process_id_t,     ipcp_id,
                void __user *,        buffer,
                port_id_t __user *,   port_id,
                size_t,               size)
{
#ifndef CONFIG_RINA
        (void) ipcp_id;
        (void) buffer;
        (void) port_id;
        (void) size;

        return -ENOSYS;
#else
        ssize_t          retval;
        struct sdu_wpi * tmp;
        size_t           retsize;

        SYSCALL_DUMP_ENTER;

        tmp = NULL;

        CALL_DEFAULT_PERSONALITY(retval, mgmt_sdu_read, ipcp_id, &tmp);
        /* Taking ownership from the internal layers */

        LOG_DBG("Personality returned value %zd", retval);

        if (retval) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        if (!sdu_wpi_is_ok(tmp)) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        /* NOTE: We don't handle partial copies */
        if (buffer_length(tmp->sdu->buffer) > size) {
                SYSCALL_DUMP_EXIT;

                LOG_ERR("Partial copies not handled. SDU size: %zd, "
                        "User space buffer size: %zd",
                        buffer_length(tmp->sdu->buffer), size);
                sdu_wpi_destroy(tmp);
                return -EFAULT;
        }

        if (copy_to_user(buffer,
                         buffer_data_ro(tmp->sdu->buffer),
                         buffer_length(tmp->sdu->buffer))) {
                SYSCALL_DUMP_EXIT;

                LOG_ERR("Error copying buffer data to user-space");
                sdu_wpi_destroy(tmp);
                return -EFAULT;
        }

        if (copy_to_user(port_id,
                         &tmp->port_id,
                         sizeof(tmp->port_id))) {
                SYSCALL_DUMP_EXIT;

                LOG_ERR("Error copying port_id data to user-space");
                sdu_wpi_destroy(tmp);
                return -EFAULT;
        }

        retsize = buffer_length(tmp->sdu->buffer);
        sdu_wpi_destroy(tmp);

        SYSCALL_DUMP_EXIT;

        return retsize;
#endif
}

SYSCALL_DEFINE5(management_sdu_write,
                ipc_process_id_t,           ipcp_id,
                address_t,                  dst_addr,
                port_id_t,                  port_id,
                const void __user *,        buffer,
                size_t,                     size)
{
#ifndef CONFIG_RINA
        (void) ipcp_id;
        (void) port_id;
        (void) buffer;
        (void) size;

        return -ENOSYS;
#else
        ssize_t          retval;
        struct sdu_wpi * sdu_wpi;
        struct buffer *  tmp_buffer;

        SYSCALL_DUMP_ENTER;

        if (!buffer || !size) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        LOG_DBG("Syscall write management SDU "
                "(size = %zd, ipcp-id = %d, port-id = %d)",
                size, ipcp_id, port_id);

        tmp_buffer = buffer_create(size);
        if (!tmp_buffer) {
                SYSCALL_DUMP_EXIT;
                return -EFAULT;
        }

        ASSERT(buffer_is_ok(tmp_buffer));
        ASSERT(buffer_data_rw(tmp_buffer));

        /* NOTE: We don't handle partial copies */
        if (copy_from_user(buffer_data_rw(tmp_buffer), buffer, size)) {
                SYSCALL_DUMP_EXIT;
                buffer_destroy(tmp_buffer);
                return -EFAULT;
        }

        /* NOTE: sdu_create takes the ownership of the buffer */
        sdu_wpi = sdu_wpi_create_with(tmp_buffer);
        if (!sdu_wpi) {
                SYSCALL_DUMP_EXIT;
                buffer_destroy(tmp_buffer);
                return -EFAULT;
        }
        sdu_wpi->dst_addr = dst_addr;
        sdu_wpi->port_id  = port_id;
        ASSERT(sdu_wpi_is_ok(sdu_wpi));

        /* Passing ownership to the internal layers */
        CALL_DEFAULT_PERSONALITY(retval, mgmt_sdu_write, ipcp_id, sdu_wpi);
        if (retval) {
                SYSCALL_DUMP_EXIT;
                /* NOTE: Do not destroy SDU, ownership isn't our anymore */
                return -EFAULT;
        }

        SYSCALL_DUMP_EXIT;

        return size;
#endif
}
