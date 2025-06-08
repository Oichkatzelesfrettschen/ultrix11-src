Build Workflow
==============

This page explains the make-based workflow used to compile Ultrix-11.

Targets
-------

* ``make`` – build user programs and the kernel.
* ``make userland`` – build only the userland sources under ``src``.
* ``make kernel`` – build the kernel located in ``sys``.
* ``make clean`` – remove build artifacts.

Cross Compilation
-----------------

Set ``CC``, ``CFLAGS``, ``AS``, and ``LDFLAGS`` to use a cross compiler or
custom toolchain when invoking ``make``.

For a step-by-step guide see :doc:`build_steps`.
