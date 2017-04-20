#! /bin/bash
make clean
make
./sim ../traces/public-assoc.trace > ../outputs/my_public-assoc.out
./sim ../traces/public-block.trace > ../outputs/my_public-block.out
./sim ../traces/public-instr.trace > ../outputs/my_public-instr.out
./sim ../traces/public-write.trace > ../outputs/my_public-write.out
./sim ../traces/spice10.trace > ../outputs/my_spice10.out
./sim ../traces/spice100.trace > ../outputs/my_spice100.out
./sim ../traces/spice1000.trace > ../outputs/my_spice1000.out

diff ../outputs/public-assoc1.out ../outputs/my_public-assoc.out
diff ../outputs/public-block1.out ../outputs/my_public-block.out
diff ../outputs/public-instr1.out ../outputs/my_public-instr.out
diff ../outputs/public-write1.out ../outputs/my_public-write.out
diff ../outputs/spice10.out ../outputs/my_spice10.out
diff ../outputs/spice100.out ../outputs/my_spice100.out
diff ../outputs/spice1000.out ../outputs/my_spice1000.out

