x86_64_v1 Architecture
=====================

The ``sys/arch/x86_64_v1`` directory contains experimental sources for
bringing Ultrix-11 to a 64-bit environment.  The code is intentionally
minimal and currently provides:

* ``boot.s`` – a very small bootstrap that jumps to ``kernel_main``.
* ``interrupts.s`` – a table of interrupt stubs that branch to a common
  handler.
* ``asm.h`` – helper macros used by the assembly sources.

These files are compiled into ``libarch.a`` and linked when building the
kernel with ``ARCH=x86_64_v1``.
