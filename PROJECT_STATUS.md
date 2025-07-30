# Project Status

As of this update, build artifacts have been removed from the repository.
All historic `build_*` directories and the `builds/` folder were purged.
Generated directories and files are ignored via `.gitignore`:

- `build/`, `build*/`, and `builds/`
- `CMakeCache.txt`, `cmake_install.cmake`, and `CMakeScripts/`
- `*/CMakeFiles/`, `*/CMakeCache.txt`, and `*/cmake_install.cmake`
- `docs/doxygen/` and `docs/sphinx/_build/`

Developers should run `scripts/build.sh` to compile the project.
