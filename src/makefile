# SCCSID: @(#)makefile	3.0	4/22/86
# Toolchain configuration
CC ?= cc
CFLAGS ?= -O2
AS ?= as
LDFLAGS ?=

export CC CFLAGS AS LDFLAGS

#
######################################################################
#   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    #
#   All Rights Reserved. 					     #
#   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      #
######################################################################
#
# makefile for directory /usr/src

CMDS =	cmd ucb etc 
CLIBS =	libc libcV libcurses libdbm libjobs libm libmp \
	libplot libtermlib libape libfpsim libPW \
	libmalloc libv7 liby
FLIBS =	libF77 libI77 libU77
# MISC = crypt games
SUBDIRS = ${CMDS} ${CLIBS} ${FLIBS} ${MISC}
HERE=/usr/src

all: clibs flibs cmds ${MISC}

dirstruct:
	-(DESTDIR=${DESTDIR}; export DESTDIR; sh mkskel)

cmds:	forceit
	-(for i in ${CMDS} ;\
	do \
	    (echo "cd $$i"; cd $$i; \
	    echo make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)" ;\
	         make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)") ;\
	done)

flibs:	forceit
	-(for i in ${FLIBS} ;\
	do \
	    (echo "cd $$i"; cd $$i ;\
	    echo make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)" ;\
	         make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)") ;\
	done)

clibs:	forceit
	-(for i in ${CLIBS} ;\
	do \
	    (echo "cd $$i"; cd $$i ;\
	    echo make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)" ;\
	         make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)") ;\
	done)

crypt:
	cd crypt; make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)"
games:
	cd games; make "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)"

sources tags sccsinfo: forceit
	-(for i in ${SUBDIRS} ;\
	do \
	    (echo "cd $$i"; cd $$i ;\
	    echo "make $@"; make $@) ;\
	done)

install: dirstruct instclibs instflibs instcmds

instcmds: forceit
	-(for i in ${CMDS} ;\
	do \
	    (echo "cd $$i"; cd $$i ;\
	    echo make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)" ;\
	    make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)") ;\
	done)

instflibs: forceit
	-(for i in ${FLIBS} ;\
	do \
	    (echo "cd $$i"; cd $$i ;\
	    echo make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)" ;\
	    make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)") ;\
	done)

instclibs: forceit
	-(for i in ${CLIBS} ;\
	do \
	    (echo "cd $$i"; cd $$i ;\
	    echo make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)" ;\
	    make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)") ;\
	done)

instmisc: forceit
	-(for i in ${MISC} ;\
	do \
	    (echo "cd $$i"; cd $$i ;\
	    echo make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)" ;\
	    make install "DESTDIR=$(DESTDIR)" "CC=$(CC)" "CFLAGS=$(CFLAGS)" "AS=$(AS)" "LDFLAGS=$(LDFLAGS)") ;\
	done)

clean clobber:	forceit
	rm -f log a.out core
	-(for i in ${SUBDIRS} ;\
	do \
	    (echo; echo "$@ing $$i"; cd $$i; make $@) ;\
	done)

forceit:
