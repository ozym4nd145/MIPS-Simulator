#! /bin/bash

TEST_DIR="./traces"
MY_OUT_DIR="./eval_results"
EXEC="../../src/cache"
rm -rf $MY_OUT_DIR
mkdir -p $MY_OUT_DIR

TABLE_HEAD="||                                                       ||          Instructions           |              Data              |        Total        ||"
TABLE_LABEL="||    CS    | I- vs D- |  BS  |   Assoc  | Write | Alloc ||  Misses   |   Repl   | Hit Rate | Misses   |  Repl    | Hit Rate |   DF     |   CB     ||"
TABLE_SEP="||-------------------------------------------------------------------------------------------------------------------------------------------------||"

###########################################################################
#Testing Given Table
###########################################################################
cd ../../src
make clean
make eval
make clean
cd -
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

###########################################################################
# Hit rate of the cache as a function of cache size
###########################################################################
echo "Hit rate of the cache as a function of cache size"

echo "Hit rate of the cache as a function of cache size" > $MY_OUT_DIR/cache_size.table

for fl in spice.trace tex.trace cc.trace
do
  cd ../../src
  make clean
  make eval
  make clean
  cd -
  echo "Processing $fl"
  echo -e "\n" >> $MY_OUT_DIR/cache_size.table
  echo "$fl" >> $MY_OUT_DIR/cache_size.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/cache_size.table
  echo "$TABLE_HEAD" >> $MY_OUT_DIR/cache_size.table
  echo "$TABLE_LABEL" >> $MY_OUT_DIR/cache_size.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/cache_size.table
  for i in {1..17}
  do
    echo -e "\tDoing for cache size $((2**($i+1)))"
    $EXEC -is $((2**($i+1))) -ds $((2**($i+1))) -bs 4 -a $((2**($i-1))) -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/cache_size.table
  done
  cd ../../src
  make clean
  make cache
  make clean
  cd -
  echo "$TABLE_SEP" >> $MY_OUT_DIR/cache_size.table   
  $EXEC -is 4 -ds 4 -bs 4 -a 1 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/cache_size.table    
  echo "$TABLE_SEP" >> $MY_OUT_DIR/cache_size.table
  echo ""
done

cat $MY_OUT_DIR/cache_size.table

###########################################################################
# Hit rate of the cache as a function of block size
###########################################################################
cd ../../src
make clean
make eval
make clean
cd -
echo "Hit rate of the cache as a function of block size"

echo "Hit rate of the cache as a function of block size" > $MY_OUT_DIR/block_size.table

for fl in spice.trace tex.trace cc.trace
do
  echo "Processing $fl"
  echo -e "\n" >> $MY_OUT_DIR/block_size.table
  echo "$fl" >> $MY_OUT_DIR/block_size.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/block_size.table
  echo "$TABLE_HEAD" >> $MY_OUT_DIR/block_size.table
  echo "$TABLE_LABEL" >> $MY_OUT_DIR/block_size.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/block_size.table
  for i in {1..11}
  do
    echo -e "\tDoing for block size $((2**($i+1)))"
    $EXEC -is 8192 -ds 8192 -bs $((2**($i+1))) -a 2 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/block_size.table
  done
  echo "$TABLE_SEP" >> $MY_OUT_DIR/block_size.table
  echo ""
done

cat $MY_OUT_DIR/block_size.table


###########################################################################
# Hit rate of the cache as a function of Assoc Num
###########################################################################
cd ../../src
make clean
make eval
make clean
cd -
echo "Hit rate of the cache as a function of associativity"

echo "Hit rate of the cache as a function of associativity" > $MY_OUT_DIR/assoc_num.table

for fl in spice.trace tex.trace cc.trace
do
  echo "Processing $fl"
  echo -e "\n" >> $MY_OUT_DIR/assoc_num.table
  echo "$fl" >> $MY_OUT_DIR/assoc_num.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/assoc_num.table
  echo "$TABLE_HEAD" >> $MY_OUT_DIR/assoc_num.table
  echo "$TABLE_LABEL" >> $MY_OUT_DIR/assoc_num.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/assoc_num.table
  for i in {0..6}
  do
    echo -e "\tDoing for Associativity $((2**($i)))"
    $EXEC -is 8192 -ds 8192 -bs 128 -a $((2**($i))) -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/assoc_num.table
  done
  echo "$TABLE_SEP" >> $MY_OUT_DIR/assoc_num.table
  echo ""
done

cat $MY_OUT_DIR/assoc_num.table


###########################################################################
# Memory Bandwidth Experiments
###########################################################################
cd ../../src
make clean
make eval
make clean
cd -
echo "Memory Bandwidth Experiments"

###########################################################################
# Comparing Write through and Write back
###########################################################################

echo -e "Comparing Write through and Write back" > $MY_OUT_DIR/mem_band_wt_wb.table

for fl in spice.trace tex.trace cc.trace
do
  echo "Processing $fl"
  echo -e "\n" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$fl" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "Write Back"
  echo "Write Back" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$TABLE_HEAD" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$TABLE_LABEL" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wt_wb.table

  $EXEC -is 8192 -ds 8192 -bs 64 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 8192 -ds 8192 -bs 64 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table

  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "Write Through"  
  echo "Write Through" >> $MY_OUT_DIR/mem_band_wt_wb.table  
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$TABLE_HEAD" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$TABLE_LABEL" >> $MY_OUT_DIR/mem_band_wt_wb.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wt_wb.table

  $EXEC -is 8192 -ds 8192 -bs 64 -a 2 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 2 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 2 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 2 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 8192 -ds 8192 -bs 64 -a 4 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 4 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 4 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 4 -wt -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wt_wb.table

  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wt_wb.table
  
  echo ""
done
cat $MY_OUT_DIR/mem_band_wt_wb.table

###########################################################################
# Comparing Write Allocate and Write no allocate
###########################################################################

echo -e "Comparing Write Allocate and Write no allocate" > $MY_OUT_DIR/mem_band_wa_wna.table

for fl in spice.trace tex.trace cc.trace
do
  echo "Processing $fl"
  echo -e "\n" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$fl" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "Write No Allocate"
  echo "Write No Allocate" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_HEAD" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_LABEL" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wa_wna.table

  $EXEC -is 8192 -ds 8192 -bs 64 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 2 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 8192 -ds 8192 -bs 64 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 4 -wb -nw $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table

  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "Write Allocate"  
  echo "Write Allocate" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_HEAD" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_LABEL" >> $MY_OUT_DIR/mem_band_wa_wna.table
  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wa_wna.table

  $EXEC -is 8192 -ds 8192 -bs 64 -a 2 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 2 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 2 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 2 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 8192 -ds 8192 -bs 64 -a 4 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 64 -a 4 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 8192 -ds 8192 -bs 128 -a 4 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table
  $EXEC -is 16384 -ds 16384 -bs 128 -a 4 -wb -wa $TEST_DIR/$fl >> $MY_OUT_DIR/mem_band_wa_wna.table

  echo "$TABLE_SEP" >> $MY_OUT_DIR/mem_band_wa_wna.table
  
  echo ""
done
cat $MY_OUT_DIR/mem_band_wa_wna.table
