#! /bin/bash
rm -rf ./results
mkdir results
make
make clean
#for i in {1..5}
#do
#echo Testcase$i
i=6
./simulator ./Testcases/testcase$i/in$i.hex ./Testcases/testcase$i/out.svg
#done