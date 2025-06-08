#!/bin/sh
# Install dependencies for building the project and its documentation.
# Prefers system packages via apt, then Python packages via pip,
# and finally Node packages via npm if needed.
set -e

# Update package list and install apt dependencies
sudo apt update
sudo apt install -y \
	doxygen \
	graphviz \
	python3-pip \
	python3-sphinx

# Install Python packages for Sphinx extensions
python3 -m pip install --user --upgrade breathe

# Placeholder for npm packages if future tooling requires them
if command -v npm >/dev/null 2>&1; then
	: # npm install <packages>
fi
