#! /bin/bash
cd ../../src
make clean
make eval
make clean
cd -

TEST_DIR="./traces"
MY_OUT_DIR="./eval_results"
EXEC="../../src/cache"
rm -rf $MY_OUT_DIR
mkdir -p $MY_OUT_DIR

$EXEC -bs 16 -us 8192 -a 1 -wb -wa $TEST_DIR/spice.trace > $MY_OUT_DIR/spice.out
