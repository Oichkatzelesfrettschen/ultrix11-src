# Project Status

As of this update, build artifacts have been removed from the repository.
Generated directories are ignored via `.gitignore`:

- `build/`, `build*/`, and `builds/`
- `*/CMakeFiles/`
- `docs/doxygen/` and `docs/sphinx/_build/`

Developers should run `scripts/build.sh` to compile the project.
