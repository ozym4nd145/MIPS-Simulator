main:	addi	$sp, $sp, -32	# Adjust stack
	sw	$ra, 24($sp)
	sw	$fp, 16($sp)	# save old frame pointer
	addi	$fp, $sp, 28	# load new frame pointer	
  lui      $1, 0x00001001 
  ori      $t0,$1,0x00000000 
  lui	  $t1, 0
  ori 	  $t1,$t1, 10			# t1(Input register) is initialized to 10.
	lui	$t0, 0		# init index to 0, Loop counter
loop:	sw	$t0, 12($sp)	# save caller saved registers
	sw	$t1, 8($sp)	# 
	move	$a0, $t0	# setup parameter for fn call
	jal	print_num	# call subroutine
	lw	$t1, 8($sp)	# restore caller saved values
	lw	$t0, 12($sp)	#
	addi	$t0, $t0, 1	# increment index;
	blt	$t0, $t1, loop	#
	lw	$fp, 16($sp)	# restore frame pointer
	lw	$ra, 24($sp)	# restore return address
	addi	$sp, $sp, 32	# restore stack pointer
	jr	$ra
print_num:	addi	$sp, $sp, -32
	sw	$fp, 16($sp)
	sw	$s0, 12($sp)
	addi	$fp, $sp, 28
	move	$s0, $a0	# store argument in temp variable
	lui	$v0, 0
	ori 	$v0,$v0, 4
  lui      $1, 0x00001001 
  ori      $a0,$1,0x00000004 
	lui	$v0, 0
	ori 	$v0,$v0, 1
	move	$a0, $s0	# restore argument $a0
	lui	$v0, 0
	ori 	$v0,$v0, 4
  lui      $1, 0x00001001 
  ori      $a0,$1,0x00000013 
	lw	$s0, 12($sp)
	lw	$fp, 16($sp)
	addi	$sp, $sp, 32
	jr	$ra		# jump to return address

