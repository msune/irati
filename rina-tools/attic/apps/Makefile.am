#
# Makefile.am
#
# Written by: Francesco Salvestrini <f DOT salvestrini AT nextworks DOT it>
#

if BUILD_TOOLS_JAVA

edit = $(SED)						\
        -e 's|@PACKAGE_VERSION[@]|$(PACKAGE_VERSION)|g'	\
        -e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g'	\
        -e 's|@PACKAGE_URL[@]|$(PACKAGE_URL)|g'		\
        -e 's|@JAVA[@]|$(JAVA)|g'			\
        -e 's|@datadir[@]|$(datadir)|g'			\
        -e 's|@pkgdatadir[@]|$(pkgdatadir)|g'		\
        -e 's|@sysconfdir[@]|$(sysconfdir)|g'		\
        -e 's|@localstatedir[@]|$(localstatedir)|g'	\
        -e 's|@libdir[@]|$(libdir)|g'			\
        -e 's|@bindir[@]|$(bindir)|g'			\
        -e 's|@sbindir[@]|$(bindir)|g'			\
	-e 's|@prefix[@]|$(prefix)|g'			\
        -e 's|@LIBRINA_VERSION[@]|$(LIBRINA_VERSION)|g'

CLEANFILES                         =
BUILT_SOURCES                      =
EXTRA_DIST                         =
bin_SCRIPTS                        =
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT =

applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-server: \
	applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-server.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	chmod a+x $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-server
BUILT_SOURCES += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-server
EXTRA_DIST    += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-server.in
bin_SCRIPTS   += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-server

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-server

applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-client: \
	applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-client.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	chmod a+x $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-client
BUILT_SOURCES += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-client
EXTRA_DIST    += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-client.in
bin_SCRIPTS   += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-client

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT += applications/rina.utils.apps.rinaband/src/main/scripts/rinaband-client

applications/rina.utils.apps.rinaband/src/main/resources/log4j.properties: \
	applications/rina.utils.apps.rinaband/src/main/resources/log4j.properties.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@


CLEANFILES    += applications/rina.utils.apps.rinaband/src/main/resources/log4j.properties
BUILT_SOURCES += applications/rina.utils.apps.rinaband/src/main/resources/log4j.properties
EXTRA_DIST    += applications/rina.utils.apps.rinaband/src/main/resources/log4j.properties.in

applications/rina.utils.apps.echo/src/main/scripts/echo-server: \
	applications/rina.utils.apps.echo/src/main/scripts/echo-server.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	chmod a+x $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.echo/src/main/scripts/echo-server
BUILT_SOURCES += applications/rina.utils.apps.echo/src/main/scripts/echo-server
EXTRA_DIST    += applications/rina.utils.apps.echo/src/main/scripts/echo-server.in
bin_SCRIPTS   += applications/rina.utils.apps.echo/src/main/scripts/echo-server
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT += applications/rina.utils.apps.echo/src/main/scripts/echo-server

applications/rina.utils.apps.echo/src/main/scripts/echo-client: \
	applications/rina.utils.apps.echo/src/main/scripts/echo-client.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	chmod a+x $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.echo/src/main/scripts/echo-client
BUILT_SOURCES += applications/rina.utils.apps.echo/src/main/scripts/echo-client
EXTRA_DIST    += applications/rina.utils.apps.echo/src/main/scripts/echo-client.in
bin_SCRIPTS   += applications/rina.utils.apps.echo/src/main/scripts/echo-client
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT += applications/rina.utils.apps.echo/src/main/scripts/echo-client

applications/rina.utils.apps.echo/src/main/resources/log4j.properties: \
	applications/rina.utils.apps.echo/src/main/resources/log4j.properties.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.echo/src/main/resources/log4j.properties
BUILT_SOURCES += applications/rina.utils.apps.echo/src/main/resources/log4j.properties
EXTRA_DIST    += applications/rina.utils.apps.echo/src/main/resources/log4j.properties.in

applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-server: \
	applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-server.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	chmod a+x $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-server
BUILT_SOURCES += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-server
EXTRA_DIST    += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-server.in
bin_SCRIPTS   += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-server
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-server

applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-client: \
	applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-client.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	chmod a+x $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-client
BUILT_SOURCES += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-client
EXTRA_DIST    += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-client.in
bin_SCRIPTS   += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-client
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT += applications/rina.utils.apps.rinaperf/src/main/scripts/rinaperf-client

applications/rina.utils.apps.rinaperf/src/main/resources/log4j.properties: \
	applications/rina.utils.apps.rinaperf/src/main/resources/log4j.properties.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

CLEANFILES    += applications/rina.utils.apps.rinaperf/src/main/resources/log4j.properties
BUILT_SOURCES += applications/rina.utils.apps.rinaperf/src/main/resources/log4j.properties
EXTRA_DIST    += applications/rina.utils.apps.rinapef/src/main/resources/log4j.properties.in

if BUILD_MAVEN_SUPPORT

pom.xml: pom.xml.in \
	Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

CLEANFILES    += pom.xml
BUILT_SOURCES += pom.xml
EXTRA_DIST    += pom.xml.in

all-local: pom.xml \
	Makefile
#	$(MVN) package

clean-local: pom.xml \
	Makefile
	$(MVN) clean

install-data-local: pom.xml \
	Makefile
	$(MVN) install

endif

endif
