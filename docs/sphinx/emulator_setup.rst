Emulator Setup
==============

The project relies on the SIMH emulator for running PDP-11 binaries.
Detailed instructions are provided in ``docs/pdp11-lab.md`` which
describes how to obtain SIMH and the operating system media.

Quick Start
-----------

After installing SIMH, create a working directory:

.. code-block:: bash

   mkdir -p ~/pdp11
   cd ~/pdp11

Copy a configuration similar to ``2.11bsd.ini`` from the lab
instructions.  Launch the emulator with:

.. code-block:: bash

   pdp11 2.11bsd.ini

From this environment you can boot 2.11BSD or install Ultrix-11
according to the steps in the lab guide.
