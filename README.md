# ultrix11-src
The Ultrix-11 3.1 source code.

## Building

The repository uses a unified Makefile system. Run `make` to build both the
userland utilities and the kernel on an x86_64 host. The main targets are:

- `make userland` – build the programs under `src/`
- `make kernel` – build the kernel sources under `sys/`
- `make clean` – remove build products
- `make docs` – build the Doxygen and Sphinx documentation

Set `CC`, `CFLAGS`, `AS`, and `LDFLAGS` to override the default toolchain.
