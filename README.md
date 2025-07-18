# ultrix11-src

The Ultrix-11 3.1 source code.

## Source Layout

| Directory | Purpose |
|-----------|---------|
| `src/`    | Userland utilities |
| `sys/`    | Kernel sources |
| `usr/`    | User programs and libraries |
| `arch/`   | Architecture-specific code |
| `etc/`    | Configuration files and scripts |
| `docs/`   | Project documentation |

## Building

The repository uses a unified Makefile system. Run `make` to build both the
userland utilities and the kernel on an x86_64 host. The main targets are:

- `make userland` – build the programs under `src/`
- `make kernel` – build the kernel sources under `sys/`
- `make clean` – remove build products
- `make docs` – build the Doxygen and Sphinx documentation

Experimental support for an `arch_x86_64_v1` build is available. Details
are documented in `docs/sphinx/arch_x86_64_v1.rst`.

Before running the above targets, execute `./setup.sh` to install the
required build tools and Python packages.

Set `CC`, `CFLAGS`, `AS`, and `LDFLAGS` to override the default toolchain.

## License

The distribution includes the original Digital Equipment Corporation
licensing terms. These terms are reproduced in the [COPYRIGHT](COPYRIGHT)
file at the repository root. The source is offered solely for historical
study and personal analysis; no additional rights are claimed.
