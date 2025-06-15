#!/bin/sh
# Install dependencies for building the project and its documentation.
# Prefers system packages via apt, then Python packages via pip,
# and finally Node packages via npm if needed.
set -e

# Ensure the package lists are current and install core dependencies
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    doxygen \
    graphviz \
    python3-pip \
    python3-sphinx

# Install Python packages for Sphinx extensions
python3 -m pip install --user --upgrade breathe

# Verify that Doxygen and Graphviz are functional
command -v doxygen >/dev/null 2>&1 || {
    echo "Error: doxygen not installed" >&2
    exit 1
}
command -v dot >/dev/null 2>&1 || {
    echo "Error: graphviz 'dot' not installed" >&2
    exit 1
}

# Placeholder for npm packages if future tooling requires them
if command -v npm >/dev/null 2>&1; then
	: # npm install <packages>
fi
