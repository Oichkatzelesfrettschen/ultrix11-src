#!/usr/bin/env bash
# Install dependencies and set up SIMH and OS images for PDP-11 development.

set -euo pipefail

# Update package lists and enable extra repositories
sudo apt update
sudo apt install -y software-properties-common
sudo add-apt-repository -y universe
sudo add-apt-repository -y multiverse
sudo apt update

# Base build stack
sudo apt install -y \
    build-essential cmake git pkg-config \
    libreadline-dev libpcap-dev zlib1g-dev \
    flex bison

# SIMH emulator
sudo apt install -y simh

# Fetch disk and tape images
mkdir -p "$HOME/pdp11/images"
cd "$HOME/pdp11/images"

# 2.11BSD distribution tape
curl -L -O https://www.tuhs.org/Archive/Distributions/UCB/2.11BSD-pl195.tar

# ULTRIX-11 tape (may fail if the mirror is unreachable)
curl -L -O https://www.tuhs.org/Archive/Distributions/DEC/Ultrix-11/3.1/ultrix-11.tap.gz || true

