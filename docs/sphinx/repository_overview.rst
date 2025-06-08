Repository Overview
===================

This page describes the layout of the Ultrix-11 source tree and its key files.

Directories
-----------

* ``src`` – userland utilities.
* ``sys`` – kernel sources.
* ``usr`` – user programs and libraries.
* ``arch`` – architecture-specific sources.
* ``etc`` – configuration files and scripts.
* ``docs`` – documentation for developers and users.

src
---

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``cmd/ac.c``
     - Accounting utility for process tracking
   * - ``libc``
     - Standard C library sources
   * - ``libF77``
     - Fortran runtime support

sys
---

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``conf/param.c``
     - Kernel parameters
   * - ``dev/cons.c``
     - Console driver
   * - ``net/if.c``
     - Network interface routines

usr
---

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``include``
     - Exported header files
   * - ``lib``
     - Libraries for user applications
   * - ``local``
     - Site-specific programs

arch
----

.. list-table:: Key components
   :header-rows: 1

   * - File
     - Purpose
   * - ``README.md``
     - Overview of architecture sources
   * - ``pdp11.md``
     - PDP-11 platform notes

Top-Level Build
---------------

A ``Makefile`` at the repository root coordinates building the sources.
The accompanying ``README.md`` provides quick build instructions.

License
-------

Original DEC licensing terms are found in the ``COPYRIGHT`` file. The
source is provided solely for historical study.
