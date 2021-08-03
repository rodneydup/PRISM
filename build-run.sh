#! /bin/sh

THREADS=$1

if [[ "$THREADS" == "" ]]; then
    echo "
   NOTE: you can append '-j#', replacing # with your CPU's number of cores plus 1, to compile faster.
   
   Example: './scripts/build.sh -j5' (for a 4-core processor)
   "
fi

# build using 8 processors
cmake --build ./build --config Release $THREADS

# Put auto-run commands here
# ./bin/PRISM
./bin/Standalone/PRISM-Dilate
