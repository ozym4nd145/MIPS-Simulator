#! /bin/bash
make clean
make
rm -rf ../my_outputs
mkdir -p ../my_outputs

./sim ../traces/public-assoc.trace > ../my_outputs/public-assoc.out
./sim ../traces/public-block.trace > ../my_outputs/public-block.out
./sim ../traces/public-instr.trace > ../my_outputs/public-instr.out
./sim ../traces/public-write.trace > ../my_outputs/public-write.out
./sim ../traces/spice10.trace > ../my_outputs/spice10.out
./sim ../traces/spice100.trace > ../my_outputs/spice100.out
./sim ../traces/spice1000.trace > ../my_outputs/spice1000.out

diff ../outputs/public-assoc1.out ../my_outputs/public-assoc.out
diff ../outputs/public-block1.out ../my_outputs/public-block.out
diff ../outputs/public-instr1.out ../my_outputs/public-instr.out
diff ../outputs/public-write1.out ../my_outputs/public-write.out
diff ../outputs/spice10.out ../my_outputs/spice10.out
diff ../outputs/spice100.out ../my_outputs/spice100.out
diff ../outputs/spice1000.out ../my_outputs/spice1000.out

