#! /bin/bash
rm -rf ./results
mkdir results
make
make clean
./simulator ./Testcases/testcase1/in1.hex ./testcases/testcase1/out.svg