# Makefile driving kernel builds
# Compiler for kernel sources
CC ?= cc
# Compiler flags for kernel
CFLAGS ?= -O2 -DKERNEL
# Assembler
AS ?= as
# Linker flags
LDFLAGS ?=

export CC CFLAGS AS LDFLAGS

SUBDIRS := conf dev distr mdec net ovdev ovnet ovsys sas sys

.PHONY: all clean

all:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d || exit 1; \
	done

clean:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean; \
	done
