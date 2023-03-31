#!/usr/bin/env bash

# https://developer.android.com/ndk/guides/cmake?hl=ko

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" || exit; pwd)
BUILD_DIR="$ROOT_DIR/cmake-build-ios-release"
INSTALL_DIR="$ROOT_DIR/demo"

if [[ ! -d "$BUILD_DIR" ]]; then
    mkdir -vp "$BUILD_DIR"
fi

"$CMAKE_CMD" \
    -S "$ROOT_DIR" \
    -B "$BUILD_DIR" \
    -DIOS=1 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
    "$ROOT_DIR"

make -C "$BUILD_DIR"
make -C "$BUILD_DIR" cc
make -C "$BUILD_DIR" install
rm -rf "$BUILD_DIR"
