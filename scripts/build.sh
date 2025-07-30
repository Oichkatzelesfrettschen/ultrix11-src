#!/usr/bin/env bash
# Build wrapper to simplify common build configurations.
set -euo pipefail

BUILD_TYPE=${1:-generic}
ARCH=${ARCH:-x86_64_v1}
BUILD_DIR=${BUILD_DIR:-build}

case "$BUILD_TYPE" in
  generic)
    CFLAGS="-O2 -Wall -Wextra -Werror"
    ;;
  performance)
    CFLAGS="-O3 -march=native -flto -Wall -Wextra -Werror"
    ;;
  developer)
    CFLAGS="-O0 -g -Wall -Wextra -Werror -fsanitize=address,undefined"
    ;;
  *)
    echo "Usage: $0 [generic|performance|developer]" >&2
    exit 1
    ;;
esac

make clean
CFLAGS="$CFLAGS" ARCH="$ARCH" make
