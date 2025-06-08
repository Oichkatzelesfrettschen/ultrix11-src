# Makefile driving kernel builds
# Compiler for kernel sources
CC ?= cc
# Compiler flags for kernel
CFLAGS ?= -O2 -DKERNEL
# Assembler
AS ?= as
# Linker flags
LDFLAGS ?=
# Target architecture (default PDP-11)
ARCH ?= pdp11

export CC CFLAGS AS LDFLAGS

SUBDIRS := conf dev distr mdec net ovdev ovnet ovsys sas sys
ifeq ($(ARCH),x86_64_v1)
SUBDIRS += arch/x86_64_v1
endif

.PHONY: all clean

all:
        @for d in $(SUBDIRS); do \
                $(MAKE) -C $$d ARCH=$(ARCH) || exit 1; \
        done

clean:
        @for d in $(SUBDIRS); do \
                $(MAKE) -C $$d clean ARCH=$(ARCH); \
        done
