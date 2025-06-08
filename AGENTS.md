# Contributor Guidelines

This repository requires strict documentation and code formatting standards.
All contributions should meet a PhD-level engineering standard for clarity and completeness.

## Documentation

- Every source file in `src/` and `sys/` **must** contain Doxygen-style
  comments for all functions, data structures, and global variables.
- When adding or modifying code, ensure the new or updated documentation
  passes `doxygen docs/Doxyfile` without warnings.
- The Sphinx manual under `docs/sphinx` should be kept in sync with the
  source code. Update reStructuredText files whenever public APIs change.
- Code lacking Doxygen comments should be brought up to compliance. Use
  `///` or `/** ... */` style annotations.
- While updating documentation, clearly highlight and comment any
  `TODO`, `PLACEHOLDER`, `FIXME`, `CRITICAL_BUG`, or `STUB` markers within
  the source. These notations should be easy to find in generated
  documentation and accompanied by an explanation of the outstanding work.

## Formatting

- Code must be formatted with `clang-format` using the style defined in
  `.clang-format` if present, or LLVM style as a default. Run
  `clang-format -i` on every modified C source/header file before
  committing.
- Formatting should only affect code, not surrounding comments.
- All code should compile cleanly with `-Wall -Wextra` and no warnings.

## Build System

All architectures **must** provide a recursive CMake build that mirrors the
traditional Makefile targets. The CMake configuration should launch via the
existing top-level `make` entry points so legacy workflows continue to
function. Maintain the CMake rules in parallel with the historical Makefiles
and keep them consistent across platforms, including the `arch_x86_64_v1`
port.

## Review Expectations

- Pull requests will be reviewed for thorough Doxygen coverage and modern
  formatting. Documentation and code quality should meet professional
  engineering standards.
- Run `make` and the documentation build steps (`doxygen` and Sphinx) to
  ensure no errors prior to submission.
