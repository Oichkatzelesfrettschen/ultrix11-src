Emulator Lab Setup
==================

The full emulator instructions reside in ``../pdp11-lab.md``.  This page
summarizes the key steps for preparing a SIMH environment.

Prerequisites
-------------

.. code-block:: bash

   sudo apt update
   sudo apt install --yes software-properties-common
   sudo add-apt-repository universe
   sudo add-apt-repository multiverse
   sudo apt install --yes simh

Fetching Media
--------------

.. code-block:: bash

   mkdir -p ~/pdp11/images
   cd ~/pdp11/images
   curl -L -O https://chasecovello.com/2.11BSD_rq.dsk.xz
   xz -dv 2.11BSD_rq.dsk.xz
   curl -L -O https://www.tuhs.org/Archive/Distributions/DEC/Ultrix-11/3.1/ultrix-11.tap.gz
   gunzip ultrix-11.tap.gz

Running
-------

.. code-block:: bash

   cd ~/pdp11
   pdp11 2.11bsd.ini

The ``scripts/setup_lab.sh`` helper automates these steps.
