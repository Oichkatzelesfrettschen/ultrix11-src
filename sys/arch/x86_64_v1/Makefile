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
