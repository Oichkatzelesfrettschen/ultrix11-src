# Makefile for the x86_64_v1 kernel build
CC ?= cc
CFLAGS ?= -O2 -DKERNEL -m64
AS ?= as
LDFLAGS ?=

export CC CFLAGS AS LDFLAGS ARCH

.PHONY: all clean

# Build the placeholder kernel
all:
@echo "Building kernel for $(ARCH)" # notify architecture
$(CC) $(CFLAGS) -c stub.c -o stub.o # compile stub
$(CC) $(LDFLAGS) stub.o -o kernel-$(ARCH) # link kernel

# Remove generated files
clean:
rm -f stub.o kernel-$(ARCH)
# Makefile for x86_64_v1 architecture
#
# Builds a small library with boot code and interrupt stubs.

ARCHIVE = libarch.a
OBJECTS = boot.o interrupts.o

CFLAGS ?= -O2 -DKERNEL -m64
ASFLAGS ?= -m64

all: $(ARCHIVE)

$(ARCHIVE): $(OBJECTS)
@rm -f $(ARCHIVE)
ar cr $(ARCHIVE) $(OBJECTS)

boot.o: boot.s
$(AS) $(ASFLAGS) -o $@ $<

interrupts.o: interrupts.s
$(AS) $(ASFLAGS) -o $@ $<

clean:
@rm -f $(OBJECTS) $(ARCHIVE)

