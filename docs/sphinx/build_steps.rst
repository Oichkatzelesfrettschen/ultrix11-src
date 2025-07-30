Build Steps
===========

This section outlines how to compile the Ultrix-11 sources using the
provided Makefiles.  The process assumes a modern Unix host with
standard development tools.

Prerequisites
-------------

Ensure a C compiler, assembler, and ``make`` are installed.  On Debian
systems this can be done via

.. code-block:: bash

   sudo apt install build-essential

Basic Build
-----------

Invoke the helper script to compile the default ``x86_64_v1`` target:

.. code-block:: bash

   ./scripts/build.sh

To build the classic PDP-11 system instead, set ``ARCH=pdp11``:

.. code-block:: bash

   ARCH=pdp11 ./scripts/build.sh

The traditional Makefile targets remain available for advanced use.

Cleaning
--------

Temporary files can be removed with:

.. code-block:: bash

   make clean

For more advanced configuration options see the top level
``README.md``.
