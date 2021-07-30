#!/bin/bash

# Make sure we're in the directory where the script is located
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$Dir"

git submodule update --init --recursive

mkdir build
mkdir -p bin/Release
mkdir -p bin/Debug

cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
