# PRISM

A suite of audio plugins (standalone and VST3) for unusual spectral processing and effects.

## PRISM-Dilate

An implementation of Spectral Dilation (DuPlessis, forthcoming). Spectral dilation applies the concept of a homothetic transformation from affine geometry to the spectrum of an input sound.

## PRISM-Mask

An implementation of Spectral Masking, which randomly masks (mutes) frequency bins in the spectrum of a sound.

## Simple Building

All of the hard stuff is done for you by the 3 included scripts.

run `./configure.sh` to init and update the submodules and create the build and bin directories. You only need to run this the first time you set up the repo or whenever you run `clean.sh`

run `./build-run.sh` to build and run your application. This puts the binary in the bin directory by default. You can use this to quickly build and run your app any time you make a change. If you are building multiple applications, you can specify which application is automatically opened after building in this script.

run `./clean.sh` to clear binaries and build stuff (you have to run configure.sh again after this).

## Requirements

Tested on Linux, probably works on Mac also.

It seems that Cmake 3.15 or newer is needed.
