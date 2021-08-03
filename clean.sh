#!/bin/bash

# Make sure we're in the directory where the script is located
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$Dir"

# remove bin and build directories
(
  rm -r -f ./bin/
  rm -r -f ./build/
)
