/*
 * RINA configuration tree
 *
 *    Vincenzo Maffione <v.maffione@nextworks.it>
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

#ifndef RINAD_RINA_CONFIGURATION_H
#define RINAD_RINA_CONFIGURATION_H

#ifdef __cplusplus

#include <librina/configuration.h>
#include <librina/common.h>

#include <string>
#include <sstream>
#include <list>
#include <map>


namespace rinad {

/*
 * Assigns an address prefix to a certain substring (the organization)
 * that is part of the application process name
 */
struct AddressPrefixConfiguration {

        static const unsigned int MAX_ADDRESSES_PER_PREFIX = 16;

        /*
         * The address prefix (it is the first valid address for the
         * given subdomain)
         */
        unsigned int addressPrefix;

        /* The organization whose addresses start by the prefix */
        std::string organization;

        AddressPrefixConfiguration() : addressPrefix(0) { }
};

struct NMinusOneFlowsConfiguration {

        /* The N-1 QoS id required by a management flow */
        int managementFlowQoSId;

        /*
         * The N-1 QoS id required by each N-1 flow  that has to be created
         * by the enrollee IPC Process after enrollment has been
         * successfully completed
         */
        std::list<int> dataFlowsQoSIds;

        NMinusOneFlowsConfiguration() : managementFlowQoSId(2) { }
};

struct ExpectedApplicationRegistration {

        std::string applicationProcessName;
        std::string applicationProcessInstance;
        std::string applicationEntityName;
        int socketPortNumber;

        ExpectedApplicationRegistration() : socketPortNumber(-1) { }
};

struct DirectoryEntry {

        std::string applicationProcessName;
        std::string applicationProcessInstance;
        std::string applicationEntityName;
        std::string hostname;
        int socketPortNumber;

        DirectoryEntry() : socketPortNumber(-1) { }
};

/* The configuration of a known IPC Process */
struct KnownIPCProcessAddress {

        /* The application name of the remote IPC Process */
        rina::ApplicationProcessNamingInformation name;

        /* The address of the remote IPC Process */
        unsigned int address;

        KnownIPCProcessAddress() : address(0) { }
};

/* The configuration required to create a DIF */
struct DIFProperties {

        rina::ApplicationProcessNamingInformation difName;
        std::string difType;
        rina::DataTransferConstants dataTransferConstants;
        std::list<rina::QoSCube> qosCubes;
        rina::RMTConfiguration rmtConfiguration;
        std::map<std::string, std::string> policies;
        std::map<std::string, std::string> policyParameters;

        /* Only for normal DIFs */
        NMinusOneFlowsConfiguration nMinusOneFlowsConfiguration;

        /* Only for shim IP DIFs */
        std::list<ExpectedApplicationRegistration> expectedApplicationRegistrations;
        std::list<DirectoryEntry> directory;

        /*
         * The addresses of the known IPC Process (apname, address)
         * that can potentially be members of the DIFs I know
         */
        std::list<KnownIPCProcessAddress> knownIPCProcessAddresses;

        /* The PDU forwarding table configurations */
        rina::PDUFTableGeneratorConfiguration pdufTableGeneratorConfiguration;

        /* The address prefixes, assigned to different organizations */
        std::list<AddressPrefixConfiguration> addressPrefixes;

        /* Extra configuration parameters (name/value pairs) */
        std::map<std::string, std::string> configParameters;

        bool lookup_ipcp_address(
                        const rina::ApplicationProcessNamingInformation&,
                        unsigned int& result);
};

struct NeighborData {

        rina::ApplicationProcessNamingInformation apName;
        rina::ApplicationProcessNamingInformation supportingDifName;
        rina::ApplicationProcessNamingInformation difName;
};


/* The configuration required to create an IPC Process */
struct IPCProcessToCreate {

        rina::ApplicationProcessNamingInformation name;
        rina::ApplicationProcessNamingInformation difName;
        std::list<NeighborData> neighbors;
        std::list<rina::ApplicationProcessNamingInformation> difsToRegisterAt;
        std::string hostname;
        /*
         * Specifies what SDU Protection Module should be used for each possible
         * N-1 DIF (being NULL the default one)
         * nMinus1DIFName: sduProtectionType
         */
        std::map<std::string, std::string> sduProtectionOptions;
        std::map<std::string, std::string> parameters;
};

/* Configuration of the local RINA Software instantiation */
struct LocalConfiguration {

        /*
         * The port where the IPC Manager is listening for incoming local
         * TCP connections from administrators
         */
        int consolePort;

        /*
         * The maximum time the CDAP state machine of a session will wait
         * for connect or release responses (in ms)
         */
        int cdapTimeoutInMs;

        /*
         * The maximum time to wait between steps of the enrollment
         * sequence (in ms)
         */
        int enrollmentTimeoutInMs;

        /*
         * The maximum number of attempts to re-enroll with a neighbor
         * with whom we've lost connectivity
         */
        int maxEnrollmentRetries;

        /*
         * The maximum time to wait to complete the flow allocation request
         * once the process has been initiated (in ms)
         */
        int flowAllocatorTimeoutInMs;

        /*
         * The period of execution of the watchdog. The watchdog send an
         * M_READ message over all the active CDAP connections to make
         * sure they are still alive.
         */
        int watchdogPeriodInMs;

        /*
         * The period after which, if no keepAlive message from a neighbor
         * IPC process has been received, it will be declared dead (and
         * adequate action will be taken)
         */
        int declaredDeadIntervalInMs;

        /*
         * The period of execution of the neighbors enroller. This task
         * looks for known neighbors in the RIB. If we're not enrolled to
         * them, he is going to try to initiate the enrollment
         */
        int neighborsEnrollerPeriodInMs;

        /* The length of Flow queues */
        int lengthOfFlowQueues;

        /* The path to the RINA binaries installation in the system */
        std::string installationPath;

        /* The path to the RINA libraries in the system */
        std::string libraryPath;

        std::string toString() const;

        LocalConfiguration() :
                        consolePort(32766),
                        cdapTimeoutInMs(10000),
                        enrollmentTimeoutInMs(10000),
                        maxEnrollmentRetries(3),
                        flowAllocatorTimeoutInMs(15000),
                        watchdogPeriodInMs(60000),
                        declaredDeadIntervalInMs(120000),
                        neighborsEnrollerPeriodInMs(10000),
                        lengthOfFlowQueues(10) { }
};

/*
 * Global configuration for the RINA software
 */
class RINAConfiguration {
 public:
        /*
         * The local software configuration (console port, timeouts, ...)
         */
        LocalConfiguration local;

        /*
         * The list of IPC Process to create when the software starts
         */
        std::list<IPCProcessToCreate> ipcProcessesToCreate;

        /*
         * The configurations of zero or more DIFs
         */
        std::list<DIFProperties> difConfigurations;

        /*
         * Application to DIF mappings
         *
         * std::string encodedAppName:
         *  The application name encoded this way:
         *  APName-APInstance-AEName-AEInstance
         *
         * rina::ApplicationProcessNamingInformation difName;
         */
        std::map<std::string,
                rina::ApplicationProcessNamingInformation>
                applicationToDIFMappings;

        bool lookup_dif_properties(
                        const rina::ApplicationProcessNamingInformation& dif_name,
                        DIFProperties& result) const;

        bool lookup_dif_by_application(
                const rina::ApplicationProcessNamingInformation& app_name,
                rina::ApplicationProcessNamingInformation& result);

        bool lookup_type_by_dif(
                const rina::ApplicationProcessNamingInformation& dif_name,
                std::string& result) const;

#if 0
        bool lookup_ipcp_address(const std::string dif_name,
                const rina::ApplicationProcessNamingInformation& ipcp_name,
                unsigned int& result);
#endif
        std::string toString() const;
};

}

#endif

#endif
