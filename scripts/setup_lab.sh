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
        flex bison \
        libgmp-dev libmpfr-dev texinfo file

# Tools required for x86_64 cross-compilation
sudo apt install -y nasm gcc-multilib

# Optional QEMU virtualization for x86_64 tests
if [[ "${INSTALL_QEMU:-0}" -eq 1 ]]; then
    sudo apt install -y qemu-system-x86
fi

# SIMH emulator
sudo apt install -y simh

# Fetch disk and tape images
mkdir -p "$HOME/pdp11/images"
cd "$HOME/pdp11/images"

# 2.11BSD distribution tape
curl -L -O https://www.tuhs.org/Archive/Distributions/UCB/2.11BSD-pl195.tar

# ULTRIX-11 tape (may fail if the mirror is unreachable)
curl -L -O https://www.tuhs.org/Archive/Distributions/DEC/Ultrix-11/3.1/ultrix-11.tap.gz || true

# Abort with a helpful message when any command fails.
trap 'echo "Setup failed." >&2' ERR

# Verify that the downloads are intact using their known SHA-256 hashes.
echo "Verifying archive checksums..."
sha256sum 2.11BSD-pl195.tar | grep -q "f95bee3dc2618c21ebf9b79557c63a0fbcd4b945807f3d90849916d542c6cfd9" || {
	echo "Checksum mismatch for 2.11BSD-pl195.tar" >&2
	exit 1
}
if [[ -f ultrix-11.tap.gz ]]; then
	sha256sum ultrix-11.tap.gz | grep -q "c971773085d9e0efb4bc4506e022752ada245cc83f231f2d0d22eb7341d7f7d5" || {
		echo "Checksum mismatch for ultrix-11.tap.gz" >&2
		exit 1
	}
fi

# Extract the archives to produce the emulator media files.
echo "Extracting archives..."
tar xf 2.11BSD-pl195.tar
[[ -f ultrix-11.tap.gz ]] && gunzip -f ultrix-11.tap.gz

# Provide a reminder about running SIMH with the downloaded images.
cat <<'EOF'

To start the PDP-11 emulator with the fetched media:
  cd ~/pdp11
  pdp11 2.11bsd.ini

EOF

echo "Setup complete."
