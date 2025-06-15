# Root Makefile to build userland and kernel
# Compiler used for all builds
CC ?= cc
# Compiler flags for C sources
CFLAGS ?= -O2
# Target architecture used for the build
# Defaults to PDP-11 for historical compatibility. Override on the
# command line for other architectures, e.g. `ARCH=x86_64_v1`.
ARCH ?= pdp11
# Assembler used for assembly sources
AS ?= as
# Additional linker flags
LDFLAGS ?=

export CC CFLAGS AS LDFLAGS ARCH

.PHONY: all userland kernel clean docs

all: userland kernel

# Build userland programs under src/
userland:
	# Propagate target architecture to the userland build
	$(MAKE) -C src ARCH=$(ARCH)

# Build kernel sources under sys/
kernel:
	# Propagate target architecture to the kernel build
	$(MAKE) -C sys ARCH=$(ARCH)

clean:
	$(MAKE) -C src clean ARCH=$(ARCH)
	$(MAKE) -C sys clean ARCH=$(ARCH)

# Build project documentation using Doxygen and Sphinx
docs:
	# Generate API documentation via Doxygen
	@doxygen docs/Doxyfile
	# Build the HTML docs with Sphinx
	@sphinx-build -b html docs/sphinx docs/sphinx/_build
