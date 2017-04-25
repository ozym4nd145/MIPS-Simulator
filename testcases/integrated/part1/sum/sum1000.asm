# http://zeta.albion.edu/~dreimann/Spring2012/courses/cs354/projects/primes.php
# Consider storing the integers 0-9 in the first ten elements of an array then reading the elements and adding them together.
.text

main:
# Fill the array

  lui      $1, 0x00000000 
  ori      $t4,$1,1000           # t4(Input register) is initialized to 1000.
#	    la       $t4, n            # address of n
#	    lw       $t4, 0($t4)       # t4 = n
	    and      $t0, $0, $0       # i = 0
#	    la       $t1, a            # address of a
  lui      $1, 0x00001001 	       # Address of a
  ori      $t1,$1,0x00000010 

loop1:
	    sll      $t2, $t0, 2       # byte offset for ith element
	    add      $t2, $t2, $t1     # address of a[i]
	    sw       $t0, 0($t2)       # put i into a[i]
	    addi     $t0, $t0, 1       # increment i
	    slt      $t5, $t0, $t4     # is $t0 < $t4 ?
	    bne      $t5, $0, loop1    # branch if so


# Sum the array values

	    and      $s0, $0, $0       # sum = 0, Result Register
	    and      $t0, $0, $0       # i = 0
	    add      $t2, $t1, $0      # address of a[i]
loop2:
	    lw       $t3, 0($t2)       # load a[i]
	    add      $s0, $s0, $t3     # increment sum
	    addi     $t0, $t0, 1       # increment i
	    addi     $t2, $t2, 4       # increment address of a[i]
	    slt      $t5, $t0, $t4     # is $t0 < $t4 ?
	    bne      $t5, $0, loop2    # branch if so

# Output Sum

            li       $v0, 1            # Load 1=print_int into $v0
            add      $a0, $s0, $zero   # Load first number into $a0
#            syscall                    # Output the prompt via syscall
					# sum (0-999) = 499500 = 0x 79F2C
            jr	$ra


.data

n:          .word  10			# n = 10
			.align 4     	# align a to start on an even word boundary
                                  	#    this is important!
a:          .space 40			# Allocate 10 words (40 bytes)

