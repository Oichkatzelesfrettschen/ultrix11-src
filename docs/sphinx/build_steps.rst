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

Run ``make`` from the repository root to build everything or invoke the
``scripts/build.sh`` helper which configures common optimization modes.

.. code-block:: bash

   make

For example, a high optimization build can be run with:

.. code-block:: bash

   scripts/build.sh performance

The ``make`` target invokes ``userland`` and ``kernel`` builds.  To
build only user programs or the kernel separately, use
``make userland`` or ``make kernel``.

An ``ARCH`` variable controls the target architecture.  It defaults to
``x86_64_v1``.  To target other platforms, pass the desired architecture
on the command line:

.. code-block:: bash

   make ARCH=pdp11

Cleaning
--------

Temporary files can be removed with:

.. code-block:: bash

   make clean

For more advanced configuration options see the top level
``README.md``.
