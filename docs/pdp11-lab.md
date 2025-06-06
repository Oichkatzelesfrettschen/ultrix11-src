# Setting Up a PDP-11 Build and Emulation Lab

This document explains how to create a development environment for Ultrix-11 and 2.11BSD using Ubuntu 24.04 LXC/Podman/Docker containers. The steps are entirely scriptable and follow modern shell practices.

## 1. Enable Additional Ubuntu Repositories

```bash
sudo apt update
sudo apt install --yes software-properties-common
sudo add-apt-repository universe
sudo add-apt-repository multiverse
sudo apt update
```

## 2. Install Base Development Tools

```bash
sudo apt install --yes \
  build-essential cmake git pkg-config \
  libreadline-dev libpcap-dev zlib1g-dev \
  flex bison
```

## 3. Install or Build SIMH

### Package Install

```bash
sudo apt install --yes simh
```

### Build Latest SIMH

```bash
git clone https://github.com/simh/simh.git
cd simh
make pdp11
sudo cp BIN/pdp11 /usr/local/bin
```

## 4. Fetch Operating System Media

```bash
mkdir -p ~/pdp11/images
cd ~/pdp11/images

curl -L -O https://chasecovello.com/2.11BSD_rq.dsk.xz
xz -dv 2.11BSD_rq.dsk.xz

curl -L -O https://www.tuhs.org/Archive/Distributions/DEC/Ultrix-11/3.1/ultrix-11.tap.gz
gunzip ultrix-11.tap.gz
```

## 5. Create a SIMH Configuration File

Create `~/pdp11/2.11bsd.ini`:

```ini
set cpu 11/93, 4M
set cpu idle
set rq0 enable, rd54, writeenabled
attach rq0 images/2.11BSD_rq.dsk
set rq1 enable, rd54, writeenabled
attach rq1 images/extra.rd54
set ts0 enable
attach ts0 images/211bsd-195.tap
set console pchar=01000023600
boot rq0
```

## 6. Launch SIMH

```bash
cd ~/pdp11
pdp11 2.11bsd.ini
```

From here you can boot 2.11BSD or switch tapes and disks to install Ultrix-11 using a similar configuration.

## 7. Optional Cross-Compiler

The host can build modern GCC/binutils for a `pdp11-aout` toolchain:

```bash
export TARGET=pdp11-aout
export PREFIX=$HOME/cross/$TARGET
mkdir -p $PREFIX

git clone https://sourceware.org/git/binutils-gdb.git
cd binutils-gdb
./configure --target=$TARGET --prefix=$PREFIX --disable-nls
make -j$(nproc)
make install
cd ..

git clone https://gcc.gnu.org/git/gcc.git
cd gcc
./configure --target=$TARGET --prefix=$PREFIX \
  --disable-nls --enable-languages=c --without-headers --with-newlib
make all-gcc -j$(nproc)
make install-gcc
```

This toolchain produces PDP-11 binaries that can be transferred to the emulator for testing.


## Automated setup

Run `scripts/setup_lab.sh` to install packages and download media automatically.


