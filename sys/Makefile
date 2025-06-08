# Makefile driving kernel builds
# Compiler for kernel sources
CC ?= cc
# Compiler flags for kernel
CFLAGS ?= -O2 -DKERNEL
# Assembler
AS ?= as
# Linker flags
LDFLAGS ?=
# Target architecture (defaults to PDP-11)
ARCH ?= pdp11

export CC CFLAGS AS LDFLAGS ARCH

SUBDIRS := conf dev distr mdec net ovdev ovnet ovsys sas sys

.PHONY: all clean

# Default PDP-11 build steps
ifeq ($(ARCH),pdp11)
all:
       @for d in $(SUBDIRS); do \
               $(MAKE) -C $$d ARCH=$(ARCH) || exit 1; \
       done

clean:
       @for d in $(SUBDIRS); do \
               $(MAKE) -C $$d clean ARCH=$(ARCH); \
       done
else
# Delegate to architecture-specific directory
all:
       $(MAKE) -C arch/$(ARCH) ARCH=$(ARCH)

clean:
       $(MAKE) -C arch/$(ARCH) clean ARCH=$(ARCH)
endif
