#!/bin/bash

set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
INCLUDE_DIR="$ROOT_DIR/include"
BUILD_DIR="$ROOT_DIR/build/macos"

export DYLD_LIBRARY_PATH=$BUILD_DIR

g++ "$ROOT_DIR/src/TEST_CLI.cpp" "$BUILD_DIR/libChessLib.dylib" -o MACOS_CLI -I"$INCLUDE_DIR"

echo "Finished compiling MACOS_CLI"