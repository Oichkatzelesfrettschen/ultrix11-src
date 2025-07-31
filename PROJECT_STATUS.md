# Project Status

As of this update, build artifacts have been removed from the repository.
Generated directories and files are ignored via `.gitignore`:

- `build/`, `build*/`, and `builds/`
- `*/CMakeFiles/`, `*/CMakeScripts/`, `CMakeCache.txt`, `cmake_install.cmake`
- `docs/doxygen/` and `docs/sphinx/_build/`

Developers should run `scripts/build.sh` to compile the project.
## Recent Cleanup
- Removed temporary `build/` directory after compilation.
- Deleted generated documentation under `docs/doxygen/` and `docs/sphinx/_build/`.
