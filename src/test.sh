#! /bin/bash
rm -rf ./results
mkdir results
make
make clean
./simulator input.txt out.svg