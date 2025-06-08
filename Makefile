# Root Makefile to build userland and kernel
# Compiler used for all builds
CC ?= cc
# Compiler flags for C sources
CFLAGS ?= -O2
# Assembler used for assembly sources
AS ?= as
# Additional linker flags
LDFLAGS ?=

export CC CFLAGS AS LDFLAGS

.PHONY: all userland kernel clean

all: userland kernel

# Build userland programs under src/
userland:
	$(MAKE) -C src

# Build kernel sources under sys/
kernel:
	$(MAKE) -C sys

clean:
	$(MAKE) -C src clean
	$(MAKE) -C sys clean
