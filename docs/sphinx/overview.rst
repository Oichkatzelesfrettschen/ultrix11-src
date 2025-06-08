Project Overview
================

This page provides a short overview of the Ultrix-11 source tree and
its documentation.  The goal of the project is to preserve the
original Ultrix-11 operating system while enabling modern build and
emulation workflows.

Source Layout
-------------

* ``src`` – userland utilities.
* ``sys`` – kernel sources.
* ``usr`` – user programs and libraries.
* ``arch`` – architecture-specific sources.
* ``etc`` – configuration files and scripts.
* ``docs`` – documentation for developers and users.
* ``sys/arch/x86_64_v1`` – experimental 64-bit architecture support.

The ``Makefile`` at the repository root coordinates building the
sources.  See :doc:`build_steps` for detailed build instructions.

src
---

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``cmd/ac.c``
     - Accounting utility for process usage
   * - ``libc``
     - Standard C library sources
   * - ``libF77``
     - Fortran runtime library

sys
---

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``conf/param.c``
     - Kernel parameter definitions
   * - ``dev/cons.c``
     - Console device driver
   * - ``net/if.c``
     - Generic network interface code

usr
---

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``include``
     - Header files installed for users
   * - ``lib``
     - Libraries linked by applications
   * - ``local``
     - Optional site utilities

arch
----

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``README.md``
     - Overview of architecture-specific sources
   * - ``pdp11.md``
     - Notes on the PDP-11 platform
