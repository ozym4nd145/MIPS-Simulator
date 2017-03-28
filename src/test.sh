#! /bin/bash
rm -rf ./results
mkdir results
make
make clean
#for i in {1..5}
#do
#echo Testcase$i
i=3
./simulator ../testcases/testcase$i/in$i.hex ../testcases/testcase$i/out.svg
#done