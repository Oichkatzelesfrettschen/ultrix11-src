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

The repository uses a unified Makefile system. A helper script
`scripts/build.sh` simplifies invoking common build types. The default
architecture is `x86_64_v1`. Run the script without arguments for a standard
build. Use `-j <jobs>` to run multiple make jobs in parallel:

```bash
./scripts/build.sh
```
Available build types are:

- `generic` – optimized for normal use
- `performance` – enables `-O3`, `-march=native`, and link-time optimizations
- `developer` – turns on sanitizers and debug information

The main Makefile targets remain available:

- `make userland` – build the programs under `src/`
- `make kernel` – build the kernel sources under `sys/`
- `make clean` – remove build products
- `make docs` – build the Doxygen and Sphinx documentation

To build the historic PDP-11 target instead, pass `ARCH=pdp11` to the
script or Makefile. Additional details on the 64-bit environment are
available in `docs/sphinx/arch_x86_64_v1.rst`.

Before running the above targets, execute `./setup.sh` to install the
required build tools and Python packages.

Set `CC`, `CFLAGS`, `AS`, and `LDFLAGS` to override the default toolchain.

## License

The distribution includes the original Digital Equipment Corporation
licensing terms. These terms are reproduced in the [COPYRIGHT](COPYRIGHT)
file at the repository root. The source is offered solely for historical
study and personal analysis; no additional rights are claimed.
