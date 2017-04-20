#! /bin/bash
rm -rf ./results
mkdir results
make
make clean
for i in {1..6}
do
echo Testcase$i
#i=3
./processor_simulator ../testcases/testcase$i/in$i.hex ../testcases/testcase$i/out.svg ../testcases/testcase$i/ourresult$i.txt
echo
echo
done
