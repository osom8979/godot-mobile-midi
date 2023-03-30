#!/usr/bin/env bash

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" || exit; pwd)
BUILD_DIR="$ROOT_DIR/cmake-build-release"
INSTALL_DIR="$ROOT_DIR/demo"

if [[ ! -d "$BUILD_DIR" ]]; then
    mkdir -vp "$BUILD_DIR"
fi

cmake \
    -S "$ROOT_DIR" \
    -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
    "$ROOT_DIR"

make -C "$BUILD_DIR"
make -C "$BUILD_DIR" cc
make -C "$BUILD_DIR" install

