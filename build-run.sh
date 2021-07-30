#! /bin/sh

# build using 8 processors
cmake --build ./build --config Release -j9

# Put auto-run commands here
# ./bin/PRISM
./bin/Standalone/PRISM-Mask
