#!/usr/bin/env bash
# Build wrapper to simplify common build configurations.
set -euo pipefail

JOBS=1
while getopts "j:" opt; do
  case "$opt" in
    j) JOBS="$OPTARG";;
    *) echo "Usage: $0 [-j jobs] [generic|performance|developer]" >&2; exit 1;;
  esac
done
shift $((OPTIND-1))

BUILD_TYPE=${1:-generic}
ARCH=${ARCH:-x86_64_v1}
BUILD_DIR=${BUILD_DIR:-build}

WARN_FLAGS=$(grep '^WARN_FLAGS' "$(dirname "$0")/build_flags.mk" | cut -d'=' -f2- | xargs)

case "$BUILD_TYPE" in
  generic)
    CFLAGS="-O2 ${WARN_FLAGS}"
    ;;
  performance)
    CFLAGS="-O3 -march=native -flto ${WARN_FLAGS}"
    ;;
  developer)
    CFLAGS="-O0 -g ${WARN_FLAGS} -fsanitize=address,undefined"
    ;;
  *)
    echo "Usage: $0 [-j jobs] [generic|performance|developer]" >&2
    exit 1
    ;;
esac
make clean
CFLAGS="$CFLAGS" ARCH="$ARCH" make -j"$JOBS"

