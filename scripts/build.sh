#!/bin/sh
# Unified build helper supporting several optimization modes.
set -e
MODE=${1:-generic}
CFLAGS=""
case "$MODE" in
  generic)
    CFLAGS="-O2 -Wall -Wextra -Werror"
    ;;
  performance)
    CFLAGS="-O3 -march=native -Wall -Wextra -Werror"
    ;;
  developer)
    CFLAGS="-O0 -g -Wall -Wextra -Werror"
    ;;
  *)
    echo "Usage: $0 [generic|performance|developer]" >&2
    exit 1
    ;;
esac

shift || true

ARCH=${ARCH:-x86_64_v1}
CFLAGS="$CFLAGS" ARCH="$ARCH" make "$@"

