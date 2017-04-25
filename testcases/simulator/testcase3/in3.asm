# Instruction 		# Details
lui $t1, 0x0000		# $t1 = 0x00000000
ori $t1, $t1, 0x4  	# $t1 = 0x00000004
lui $t2, 0x0000		# $t2 = 0x00000000
ori $t2, $t2, 0x6  	# $t2 = 0x00000006
lui $t3, 0x0000		# $t3 = 0x00000000
ori $t3, $t3, 0x2  	# $t3 = 0x00000002
add  $t1, $t2, $t3    	# $t1 = $t2 + $t3 $t1 = 0x00000008
sub  $t1, $t1, $t3    	# $t1 = $t1 - $t3 $t1 = 0x00000006 (Forwarding)
and  $t1, $t2, $t3    	# $t1 = $t2 & $t3 (bitwise and) $t1 = 0x00000002
or   $t1, $t1, $t3    	# $t1 = $t1 | $t3 (bitwise or)  $t1 = 0x00000006  (Forwarding)
lui $t1, 0x0000		# $t1 = 0x00000000
ori $t1, $t1, 0xC  	# $t1 = 0x0000000C
mult $t1, $t2         	# hi,lo = $t1*$t2 hi = 0x00000000 lo = 0x00000048	
lui $t5, 0x1001		# $t5 = 0x10010000
sw $t1, 4($t5) 	# Memory[$t5+4] = $t1  Mem[268500996]= 0x0000000C
lw $t4, 4($t5) 	# $t4 = Memory[$t5+400]  $t4 = 0x0000000C
add  $t1, $t4, $t3    	# $t1 = $t4 + $t3 $t1 = 0x0000000E (Hazard, Stall)
nor $t1, $t2, $t3    	# $t1 = ~($t2 | $t3) (bitwise nor)  $t1 = 0xFFFFFFF9
madd $t3, $t2         	# hi,lo = (HI,LO)+$t3*$t2 = 0x00000048+0x0000000C = 0x00000054, hi = 0x00000000 lo = 0x00000054
sll $t1, $t2, 3		# $t1 = $t2 << 3 = 0x00000006 << 3 = 0x00000030
sllv $t1, $t2, $t3	# $t1 = $t2 << $t3 = 0x00000006 << 2 = 0x00000018
sltu $t1, $t2, $t3	# $t1 = ($t2 < $t3)?1:0 = (6 < 2)?1:0 = 0x00000000
slti $t1, $t1, 7	# $t1 = ($t1 < 7)?1:0 = (1 < 7)?1:0 = 0x00000001 (Forwarding)