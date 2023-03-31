#!/usr/bin/env bash

# https://developer.android.com/ndk/guides/cmake?hl=ko

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" || exit; pwd)
BUILD_DIR="$ROOT_DIR/cmake-build-android-release"
INSTALL_DIR="$ROOT_DIR/demo"
CMAKE_CMD="$ROOT_DIR/android_sdk/cmake/3.22.1/bin/cmake"
NDK_DIR="${ROOT_DIR}/android_sdk/ndk/25.2.9519653"
ANDROID_TOOLCHAIN="${NDK_DIR}/build/cmake/android.toolchain.cmake"

ANDROID_ABI=armeabi-v7a
#ANDROID_ABI="armeabi-v7a with NEON"
#ANDROID_ABI=arm64-v8a
#ANDROID_ABI=x86
#ANDROID_ABI=x86_64

MINSDKVERSION=23
ANDROID_PLATFORM=android-${MINSDKVERSION}

if [[ ! -d "$BUILD_DIR" ]]; then
    mkdir -vp "$BUILD_DIR"
fi

"$CMAKE_CMD" \
    -S "$ROOT_DIR" \
    -B "$BUILD_DIR" \
    -DANDROID_ABI="$ANDROID_ABI" \
    -DANDROID_PLATFORM="$ANDROID_PLATFORM" \
    -DCMAKE_TOOLCHAIN_FILE="$ANDROID_TOOLCHAIN" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
    "$ROOT_DIR"

make -C "$BUILD_DIR"
make -C "$BUILD_DIR" cc
make -C "$BUILD_DIR" install
rm -rf "$BUILD_DIR"
