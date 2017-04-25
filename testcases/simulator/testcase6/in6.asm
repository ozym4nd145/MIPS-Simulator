# Let the variable i  be stored in register $4
# Let ‘int array’ start  at address 0x10010000
# Each integer occupies 4 addresses
# Assuming memory and registers are initialized to zero.

add $4, $0, $0 		#  set  $4=0 # 0 = i
loop:	
slti $8, $4, 10 	# set $8=1 if $4 < 10 otherwise $8=0
beq $8, $0, end		# if $8=0 ($4>=10) branch to end label
lui $8, 0x1001		# $8 = 0x10010000
sll $9, $4, 2		# $9 = $4 << 2  # $9 = i*4 
add $8, $8, $9		# form address of array[i] in $8
sw $4, 0($8)		# store 32-bits of i into array[i]
addi $4, $4, 1		# i++  
beq $0, $0, loop	# branch to label loop
end:
