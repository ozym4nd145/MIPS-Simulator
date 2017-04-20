#! /bin/bash
make clean
make
make clean
# rm -rf ../my_outputs
# mkdir -p ../my_outputs

# ./sim ../traces/public-assoc.trace > ../my_outputs/public-assoc.out
# ./sim ../traces/public-block.trace > ../my_outputs/public-block.out
# ./sim ../traces/public-instr.trace > ../my_outputs/public-instr.out
# ./sim ../traces/public-write.trace > ../my_outputs/public-write.out
# ./sim ../traces/spice10.trace > ../my_outputs/spice10.out
# ./sim ../traces/spice100.trace > ../my_outputs/spice100.out
# ./sim ../traces/spice1000.trace > ../my_outputs/spice1000.out

# diff ../outputs/public-assoc1.out ../my_outputs/public-assoc.out
# diff ../outputs/public-block1.out ../my_outputs/public-block.out
# diff ../outputs/public-instr1.out ../my_outputs/public-instr.out
# diff ../outputs/public-write1.out ../my_outputs/public-write.out
# diff ../outputs/spice10.out ../my_outputs/spice10.out
# diff ../outputs/spice100.out ../my_outputs/spice100.out
# diff ../outputs/spice1000.out ../my_outputs/spice1000.out

###########################################################################
#Better Testing
###########################################################################
TEST_DIR="../more_test/traces"
OUT_DIR="../more_test/outputs"
MY_OUT_DIR="../more_test/my_outputs"
rm -rf $MY_OUT_DIR
mkdir -p $MY_OUT_DIR

./sim $TEST_DIR/public-assoc.trace > $MY_OUT_DIR/public-assoc1.out
./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/public-assoc.trace > $MY_OUT_DIR/public-assoc2.out
./sim -bs 16 -us 256 -a 2 -wb -wa $TEST_DIR/public-assoc.trace > $MY_OUT_DIR/public-assoc3.out
./sim -bs 16 -us 256 -a 4 -wb -wa $TEST_DIR/public-assoc.trace > $MY_OUT_DIR/public-assoc4.out
./sim -bs 16 -us 256 -a 8 -wb -wa $TEST_DIR/public-assoc.trace > $MY_OUT_DIR/public-assoc5.out
echo "Public Assoc"
echo "public-assoc1.out"
diff $OUT_DIR/public-assoc1.out $MY_OUT_DIR/public-assoc1.out
echo "public-assoc2.out"
diff $OUT_DIR/public-assoc2.out $MY_OUT_DIR/public-assoc2.out
echo "public-assoc3.out"
diff $OUT_DIR/public-assoc3.out $MY_OUT_DIR/public-assoc3.out
echo "public-assoc4.out"
diff $OUT_DIR/public-assoc4.out $MY_OUT_DIR/public-assoc4.out
echo "public-assoc5.out"
diff $OUT_DIR/public-assoc5.out $MY_OUT_DIR/public-assoc5.out
echo "-------------------------------------------------------"

./sim $TEST_DIR/public-block.trace > $MY_OUT_DIR/public-block1.out
./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/public-block.trace > $MY_OUT_DIR/public-block2.out
./sim -bs 8 -us 256 -a 1 -wb -wa $TEST_DIR/public-block.trace > $MY_OUT_DIR/public-block3.out
./sim -bs 4 -us 256 -a 1 -wb -wa $TEST_DIR/public-block.trace > $MY_OUT_DIR/public-block4.out
./sim -bs 32 -us 256 -a 1 -wb -wa $TEST_DIR/public-block.trace > $MY_OUT_DIR/public-block5.out
echo "Public Block"
echo "public-block1.out"
diff $OUT_DIR/public-block1.out $MY_OUT_DIR/public-block1.out
echo "public-block2.out"
diff $OUT_DIR/public-block2.out $MY_OUT_DIR/public-block2.out
echo "public-block3.out"
diff $OUT_DIR/public-block3.out $MY_OUT_DIR/public-block3.out
echo "public-block4.out"
diff $OUT_DIR/public-block4.out $MY_OUT_DIR/public-block4.out
echo "public-block5.out"
diff $OUT_DIR/public-block5.out $MY_OUT_DIR/public-block5.out
echo "-------------------------------------------------------"


./sim $TEST_DIR/public-instr.trace > $MY_OUT_DIR/public-instr1.out
./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/public-instr.trace > $MY_OUT_DIR/public-instr2.out
./sim -bs 16 -us 512 -a 1 -wb -wa $TEST_DIR/public-instr.trace > $MY_OUT_DIR/public-instr3.out
./sim -bs 16 -is 128 -ds 128 -a 1 -wb -wa $TEST_DIR/public-instr.trace > $MY_OUT_DIR/public-instr4.out
./sim -bs 16 -is 256 -ds 256 -a 1 -wb -wa $TEST_DIR/public-instr.trace > $MY_OUT_DIR/public-instr5.out
echo "Public Instr"
echo "public-instr1.out"
diff $OUT_DIR/public-instr1.out $MY_OUT_DIR/public-instr1.out
echo "public-instr2.out"
diff $OUT_DIR/public-instr2.out $MY_OUT_DIR/public-instr2.out
echo "public-instr3.out"
diff $OUT_DIR/public-instr3.out $MY_OUT_DIR/public-instr3.out
echo "public-instr4.out"
diff $OUT_DIR/public-instr4.out $MY_OUT_DIR/public-instr4.out
echo "public-instr5.out"
diff $OUT_DIR/public-instr5.out $MY_OUT_DIR/public-instr5.out
echo "-------------------------------------------------------"



./sim $TEST_DIR/public-write.trace > $MY_OUT_DIR/public-write1.out
./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/public-write.trace > $MY_OUT_DIR/public-write2.out
./sim -bs 16 -us 256 -a 1 -wt -wa $TEST_DIR/public-write.trace > $MY_OUT_DIR/public-write3.out
./sim -bs 16 -us 256 -a 1 -wb -nw $TEST_DIR/public-write.trace > $MY_OUT_DIR/public-write4.out
echo "Public Write"
echo "public-write1.out"
diff $OUT_DIR/public-write1.out $MY_OUT_DIR/public-write1.out
echo "public-write2.out"
diff $OUT_DIR/public-write2.out $MY_OUT_DIR/public-write2.out
echo "public-write3.out"
diff $OUT_DIR/public-write3.out $MY_OUT_DIR/public-write3.out
echo "public-write4.out"
diff $OUT_DIR/public-write4.out $MY_OUT_DIR/public-write4.out
echo "-------------------------------------------------------"

./sim $TEST_DIR/spice10.trace > $MY_OUT_DIR/spice10.out
./sim $TEST_DIR/spice100.trace > $MY_OUT_DIR/spice100.out
./sim $TEST_DIR/spice1000.trace > $MY_OUT_DIR/spice1000.out
echo "Spice 10 100 1000"
echo "spice10.out"
diff $OUT_DIR/spice10.out $MY_OUT_DIR/spice10.out
echo "spice100.out"
diff $OUT_DIR/spice100.out $MY_OUT_DIR/spice100.out
echo "spice1000.out"
diff $OUT_DIR/spice1000.out $MY_OUT_DIR/spice1000.out
echo "-------------------------------------------------------"

./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-assoc2.out
./sim -bs 16 -us 256 -a 2 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-assoc3.out
./sim -bs 16 -us 256 -a 4 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-assoc4.out
./sim -bs 16 -us 256 -a 8 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-assoc5.out
./sim -bs 16 -us 8192 -a 2 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-assoc6.out
./sim -bs 16 -us 8192 -a 4 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-assoc7.out
./sim -bs 16 -us 8192 -a 8 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-assoc8.out
echo "Spice 100000 Assoc"
echo "spice10000-assoc2.out"
diff $OUT_DIR/spice10000-assoc2.out $MY_OUT_DIR/spice10000-assoc2.out
echo "spice10000-assoc3.out"
diff $OUT_DIR/spice10000-assoc3.out $MY_OUT_DIR/spice10000-assoc3.out
echo "spice10000-assoc4.out"
diff $OUT_DIR/spice10000-assoc4.out $MY_OUT_DIR/spice10000-assoc4.out
echo "spice10000-assoc5.out"
diff $OUT_DIR/spice10000-assoc5.out $MY_OUT_DIR/spice10000-assoc5.out
echo "spice10000-assoc6.out"
diff $OUT_DIR/spice10000-assoc6.out $MY_OUT_DIR/spice10000-assoc6.out
echo "spice10000-assoc7.out"
diff $OUT_DIR/spice10000-assoc7.out $MY_OUT_DIR/spice10000-assoc7.out
echo "spice10000-assoc8.out"
diff $OUT_DIR/spice10000-assoc8.out $MY_OUT_DIR/spice10000-assoc8.out
echo "-------------------------------------------------------"

./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-write2.out
./sim -bs 16 -us 256 -a 1 -wt -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-write3.out
./sim -bs 16 -us 256 -a 1 -wb -nw $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-write4.out
echo "Spice 100000 Write"
echo "spice10000-write2.out"
diff $OUT_DIR/spice10000-write2.out $MY_OUT_DIR/spice10000-write2.out
echo "spice10000-write3.out"
diff $OUT_DIR/spice10000-write3.out $MY_OUT_DIR/spice10000-write3.out
echo "spice10000-write4.out"
diff $OUT_DIR/spice10000-write4.out $MY_OUT_DIR/spice10000-write4.out
echo "-------------------------------------------------------"

./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-instr2.out
./sim -bs 16 -us 512 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-instr3.out
./sim -bs 16 -is 128 -ds 128 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-instr4.out
./sim -bs 16 -is 256 -ds 256 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-instr5.out
./sim -bs 16 -is 1024 -ds 1024 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-instr6.out
./sim -bs 16 -is 4096 -ds 4096 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-instr7.out
./sim -bs 16 -is 8192 -ds 8192 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-instr8.out
echo "Spice 100000 Instr"
echo "spice10000-instr2.out"
diff $OUT_DIR/spice10000-instr2.out $MY_OUT_DIR/spice10000-instr2.out
echo "spice10000-instr3.out"
diff $OUT_DIR/spice10000-instr3.out $MY_OUT_DIR/spice10000-instr3.out
echo "spice10000-instr4.out"
diff $OUT_DIR/spice10000-instr4.out $MY_OUT_DIR/spice10000-instr4.out
echo "spice10000-instr5.out"
diff $OUT_DIR/spice10000-instr5.out $MY_OUT_DIR/spice10000-instr5.out
echo "spice10000-instr6.out"
diff $OUT_DIR/spice10000-instr6.out $MY_OUT_DIR/spice10000-instr6.out
echo "spice10000-instr7.out"
diff $OUT_DIR/spice10000-instr7.out $MY_OUT_DIR/spice10000-instr7.out
echo "spice10000-instr8.out"
diff $OUT_DIR/spice10000-instr8.out $MY_OUT_DIR/spice10000-instr8.out
echo "-------------------------------------------------------"

./sim -bs 16 -us 256 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-block2.out
./sim -bs 8 -us 256 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-block3.out
./sim -bs 4 -us 256 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-block4.out
./sim -bs 32 -us 8192 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-block5.out
./sim -bs 64 -us 8192 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-block6.out
./sim -bs 128 -us 8192 -a 1 -wb -wa $TEST_DIR/spice10000.trace > $MY_OUT_DIR/spice10000-block7.out
echo "Spice 100000 Block"
echo "spice10000-block2.out"
diff $OUT_DIR/spice10000-block2.out $MY_OUT_DIR/spice10000-block2.out
echo "spice10000-block3.out"
diff $OUT_DIR/spice10000-block3.out $MY_OUT_DIR/spice10000-block3.out
echo "spice10000-block4.out"
diff $OUT_DIR/spice10000-block4.out $MY_OUT_DIR/spice10000-block4.out
echo "spice10000-block5.out"
diff $OUT_DIR/spice10000-block5.out $MY_OUT_DIR/spice10000-block5.out
echo "spice10000-block6.out"
diff $OUT_DIR/spice10000-block6.out $MY_OUT_DIR/spice10000-block6.out
echo "spice10000-block7.out"
diff $OUT_DIR/spice10000-block7.out $MY_OUT_DIR/spice10000-block7.out
echo "-------------------------------------------------------"
