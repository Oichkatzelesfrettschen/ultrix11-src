# Documentation Build Instructions

This directory contains the documentation framework for Ultrix-11.
The documentation is generated using **Doxygen** and **Sphinx**.

## Building Doxygen Docs

1. Install Doxygen and Graphviz: `sudo apt install doxygen graphviz`.
2. From the repository root, run `doxygen docs/Doxyfile`.
3. The HTML output will appear under `docs/doxygen/html`.

## Building Sphinx Docs

1. Install Sphinx and the Breathe extension:
   `pip install sphinx sphinx-autobuild breathe`.
2. Ensure `doxygen` has been run so that XML output exists in `docs/doxygen/xml`.
3. Change into `docs/sphinx` and run `sphinx-build -b html . _build`.
4. View the generated site in `_build/index.html`.
