Architecture: x86_64 v1

This document outlines the build prerequisites for the experimental
``arch_x86_64_v1`` port, describes the system layout for this target and
summarizes how it differs from the classic PDP-11 environment.

Prerequisites
-------------

* A modern 64-bit Linux distribution with ``gcc``, ``make`` and
  ``binutils`` installed.
* Python 3.10 or newer with the ``breathe`` and ``sphinx`` packages.
* ``doxygen`` for generating API documentation.

Layout
------

The ``arch_x86_64_v1`` directory mirrors the traditional layout used for
PDP-11 sources:

* ``src`` – userland utilities compiled for ``x86_64``.
* ``sys`` – kernel sources targeting the new architecture.
* ``usr`` and ``etc`` retain their original roles for system libraries and
  configuration files.

Differences from PDP-11
-----------------------

* 64-bit pointers and word sizes require updates to many legacy data
  structures.
* The boot process uses the standard ``grub`` loader rather than PDP-11
  boot ROMs.
* Endianness is little-endian instead of the PDP-11's native byte order.

This port is very early in development. See :doc:`build_steps` for how to
invoke the build system. The root ``README.md`` provides additional
context and links to emulator notes.

x86_64_v1 Architecture

The ``sys/arch/x86_64_v1`` directory contains experimental sources for
bringing Ultrix-11 to a 64-bit environment.  The code is intentionally
minimal and currently provides:

* ``boot.s`` – a very small bootstrap that jumps to ``kernel_main``.
* ``interrupts.s`` – a table of interrupt stubs that branch to a common
  handler.
* ``asm.h`` – helper macros used by the assembly sources.

These files are compiled into ``libarch.a`` and linked when building the
kernel with ``ARCH=x86_64_v1``.

