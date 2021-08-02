#!/bin/bash

# Make sure we're in the directory where the script is located
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$Dir"

# update submodules (juce)
git submodule update --init --recursive

# make the build directory
mkdir build

# make directories for binaries, because we told cmake to put binaries here for ease of access
mkdir -p bin/Release
mkdir -p bin/Debug

# go into the build directory
cd build

# run the cmake configuration
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
