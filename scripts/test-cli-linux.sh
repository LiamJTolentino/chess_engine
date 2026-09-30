#!/bin/bash

set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
INCLUDE_DIR="$ROOT_DIR/include"
BUILD_DIR="$ROOT_DIR/build/linux"

echo "$BUILD_DIR/libChessLib.so"
export LD_LIBRARY_PATH=$BUILD_DIR

g++ "$ROOT_DIR/src/TEST_CLI.cpp" "$BUILD_DIR/libChessLib.so" -o CLI_TEST -I"$INCLUDE_DIR"

