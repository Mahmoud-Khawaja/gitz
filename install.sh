#!/bin/bash
set -e

mkdir -p build

cd build

cmake ..

make

sudo make install

echo "gitz has been installed to /usr/local/bin"
echo "you can now use it by running 'gitz <command> [<args>]'"
echo "happy hawktuah nigga!"