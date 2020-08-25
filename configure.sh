#!/bin/bash

# Make sure we're in the directory where the script is located
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$Dir"

git submodule update --init --recursive

mkdir build
mkdir bin

cd build
cmake ..
