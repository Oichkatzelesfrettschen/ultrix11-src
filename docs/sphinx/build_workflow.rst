Build Workflow
==============

This page explains the CMake-based workflow used to compile Ultrix-11.  The
``Makefile`` still acts as a convenient wrapper so traditional commands
continue to function.

Targets
-------

* ``make`` – configure CMake and build everything.
* ``make userland`` – build only the userland targets.
* ``make kernel`` – build only the kernel.
* ``make clean`` – remove the ``build`` directory.
* ``cmake -S . -B build`` – manual configuration step if not using ``make``.
* ``cmake --build build --target <name>`` – build specific targets.

Cross Compilation
-----------------

Set ``CC``, ``CFLAGS``, ``AS``, and ``LDFLAGS`` to use a cross compiler or
custom toolchain when invoking ``make``.

For a step-by-step guide see :doc:`build_steps`.
