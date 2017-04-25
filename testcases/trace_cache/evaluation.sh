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

TABLE_HEAD="||                                                 ||     Instructions     |         Data        |        Total        ||"
TABLE_LABEL="||   CS   | I- vs D- |  BS  |  Ass | Write | Alloc ||  Misses   |   Repl   | Misses   |  Repl    |   DF     |   CB     ||"
TABLE_SEP="||---------------------------------------------------------------------------------------------------------------------||"

# Testing given table
echo "Testing Given table"

echo "$TABLE_SEP" > $MY_OUT_DIR/test_given.table
echo "$TABLE_HEAD" >> $MY_OUT_DIR/test_given.table
echo "$TABLE_LABEL" >> $MY_OUT_DIR/test_given.table
echo "$TABLE_SEP" >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 16 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 16384 -ds 16384 -bs 16 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 32768 -ds 32768 -bs 16 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 65536 -ds 65536 -bs 16 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -us 8192 -bs 16 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -us 8192 -bs 32 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -us 8192 -bs 64 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 32768 -ds 32768 -bs 128 -a 1 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 64 -a 2 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 64 -a 4 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 64 -a 8 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 64 -a 16 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 64 -a 128 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 1024 -ds 1024 -bs 64 -a 2 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 1024 -ds 1024 -bs 64 -a 8 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 1024 -ds 1024 -bs 64 -a 16 -wb -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 16 -a 1 -wt -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 32 -a 1 -wt -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 64 -a 2 -wt -wa $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 16 -a 1 -wb -nw $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 32 -a 1 -wb -nw $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table
$EXEC -is 8192 -ds 8192 -bs 64 -a 2 -wb -nw $TEST_DIR/spice.trace >> $MY_OUT_DIR/test_given.table

echo "$TABLE_SEP" >> $MY_OUT_DIR/test_given.table

cat $MY_OUT_DIR/test_given.table
