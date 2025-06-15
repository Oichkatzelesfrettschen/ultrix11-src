# Root Makefile to build userland and kernel
# Compiler used for all builds
CC ?= cc
# Compiler flags for C sources
CFLAGS ?= -O2
# Target architecture used for the build
# Defaults to PDP-11 for historical compatibility. Override on the
# command line for other architectures, e.g. `ARCH=x86_64_v1`.
ARCH ?= pdp11
# Target architecture
#ARCH ?= x86_64_v1

# Assembler used for assembly sources
AS ?= as
# Additional linker flags
LDFLAGS ?=

export CC CFLAGS AS LDFLAGS ARCH

.PHONY: all userland kernel clean docs
	
BUILD_DIR ?= build
	
CONFIGURE_CMD = \
cmake -S . -B $(BUILD_DIR) \
-DARCH=$(ARCH) \
-DCMAKE_C_FLAGS="$(CFLAGS)" \
-DCMAKE_EXE_LINKER_FLAGS="$(LDFLAGS)"

all: configure
	cmake --build $(BUILD_DIR)

# Build userland programs under src/
userland: configure
	cmake --build $(BUILD_DIR) --target userland

# Build kernel sources under sys/
kernel: configure
	cmake --build $(BUILD_DIR) --target kernel

configure:
	$(CONFIGURE_CMD)

clean:
	rm -rf $(BUILD_DIR)

# Build project documentation using Doxygen and Sphinx
docs:
	# Generate API documentation via Doxygen
	@doxygen docs/Doxyfile
	# Build the HTML docs with Sphinx
	@sphinx-build -b html docs/sphinx docs/sphinx/_build
