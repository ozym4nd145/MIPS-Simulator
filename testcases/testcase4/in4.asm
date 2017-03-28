# See: http://www0.cs.ucl.ac.uk/staff/electran/gc03/pdf/07mips_examples.pdf
# int j = 10 ;  // space must be allocated to variable j (assume address 0x10010000)
lui $1, 0x1001		# $t5 = 0x10010000
addi $8, $0, 10		# $8 = $0 + sign-extend[10] Get 10 in $8 
sw $8, 0($1)		# Mem[$1 + 0] = $8 Store 10 = 0x12345678
