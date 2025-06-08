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
* ``etc`` – configuration files and scripts.
* ``docs`` – documentation for developers and users.

The ``Makefile`` at the repository root coordinates building the
sources.  See :doc:`build_steps` for detailed build instructions.
