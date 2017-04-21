# Program writes/reads A from 4 bytes of memory starting at address 0x10010000
# Program writes/reads B from 4 bytes of memory starting at address 0x10010004
# Program writes |A-B| to 4 bytes of memory starting at address 0x10010008

lui $10, 0x1001		#  put address of A into register $10
lui $t6, 0x0001		#  Initialize the data memory $t6 = 0x00010000
lui $t7, 0x0002		#  $t7 = 0x00020000
sw $t6, 0($10) 		#  A = 0x00010000
sw $t7, 4($10) 		#  B = 0x00020000

lw $4, 0($10)		#  read A from memory into register $4
lw $5, 4($10)		#  read B from memory into register $5 (A address+4)
sub $12, $4, $5 	#  subtract B from A => A-B into register $12
bgez   $12,end		#  branch if A-B is positive to 'sw' instruction
sub    $12, $5, $4 	#  subtract A from B => B-A into register $12
end:
sw $12, 8($10) 		#  store register $12 value, |A-B|, into memory
