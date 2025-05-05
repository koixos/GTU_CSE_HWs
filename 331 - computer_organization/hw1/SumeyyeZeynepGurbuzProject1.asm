.data
	Str1:			.asciiz "Line: "
	Str2:			.asciiz "Row size: "
	Str3:			.asciiz "Column size: "
	Str4:			.asciiz "Step number: "
	Str5:			.asciiz "Enter a line: "
	
	Row:			.word 16	# row size, default 16
	Col:			.word 16	# column size, default 16
	Step:			.word 3		# step counter, default 3
	
	Grid:			.word 0		# main grid, initially null
	MemGrid:		.word 0		# memory grid, initially null

	.eqv DATA_SIZE	1 			# sizeOf(char) = 1 byte
	
.text
main:							# main function
	jal takeInput				# take start inputs from usr
	jal createGrid				# create main grid
	jal firstStep				# create memory grid
	jal printGrid				# print initial state of the main grid

	lw $s0, Step				# step counter --> s0-reg
	add $s1, $zero, 2			# (stepCounter = 2) --> s1-reg (2 sec passed)
	
	Step_Loop:
	bgt $s1, $s0, Exit			# while (stepCounter <= Step)
	
	jal secondStep				# plant bomb everywhere in the grid
	add $s1, $s1, 1				# another sec has passed
	
	bgt $s1, $s0, Exit			# if (stepCounter > Step) goto Exit;
	
	jal thirdStep				# bomb the bombs planted 3 sec ago
	jal firstStep				# update memory grid
	add $s1, $s1, 1				# another sec has passed
	
	j Step_Loop 

	Exit:
	jal printGrid				# print final state of the main grid
	
	li $v0, 10					# exit
	syscall
		
takeInput:						# function to take row & column size, and step counter
	la $a0, Str2
	li $v0, 4					# print Str2
	syscall
	
	li $v0, 5					# read row size
	syscall
	sw $v0, Row					# row size --> Row
	
	la $a0, Str3
	li $v0, 4					# print Str3
	syscall
	
	li $v0, 5					# read column size
	syscall
	sw $v0, Col					# col size --> Col
	
	la $a0, Str4
	li $v0, 4					# print Str4
	syscall
	
	li $v0, 5					# read step counter
	syscall
	sw $v0, Step				# step counter --> Step

	li $a0, 10					# a0-reg: new-line ascii value
	li $v0, 11					# print new-line for readability
	syscall
	
	jr $ra						# jump back to the calling code
	
createGrid:						# function to create & initialize main grid w/ given inputs						
	lw $t0, Row 				# Row --> t0-reg
	lw $t1, Col					# Col --> t1-reg
								# dynamically allocating mem for the matrix
	mul $a0, $t0, $t1 			# num of elements = row * column
	mul $a0, $a0, DATA_SIZE 	# total size = (num of elements) * DATA_SIZE
	li $v0, 9 					# heap allocation
	syscall
	move $t2, $v0				# t2-reg --> allocated mem addr			
	sw $t2, Grid 				# Grid: allocated mem addr	
								# create string buffer to get lines from user 
	add $a0, $t1, 1				# total size for a line = column + 1(EOF)
	li $v0, 9					# heap allocation
	syscall
	move $t3, $v0				# t3-reg: addr of buff		
	
	add $t4, $zero, $zero		# (rowCounter = 0) --> t4-reg
	
	Row_Loop:						
	beq $t4, $t0, Break			# while (rowCounter != Row)
	
	la $a0, Str5				
	li $v0, 4					# print Str5
	syscall
	
	la $a0, ($t3)				# a0-reg: addr of buff
	add $a1, $t1, 1				# a1-reg: max char num to read
	li $v0, 8					# get a line from user
	syscall
					
	li $a0, 10					# a0-reg --> new-line ASCII value
	li $v0, 11					# print new-line for readability
	syscall
	
	add $t5, $zero, $zero		# (colCounter = 0) --> t5-reg
	
	Col_Loop:						
	beq $t5, $t1, Next_Row		# while (colCounter != Col)
								# calculate index: index = (rowCounter * Col) + colCounter
	mul $t6, $t4, $t1			# t6-reg --> rowCounter * Col
	add $t6, $t6, $t5			# t6-reg --> (rowCounter * Col) + colCounter
	mul $t6, $t6, DATA_SIZE		# t6-reg --> index * DATA_SIZE
	add $t6, $t6, $t2			# t6-reg --> addr in memory
	
	lb $t7, 0($t3)				# t7-reg --> char at curr index of the given line
	sb $t7, 0($t6)				# t6-reg: char at the curr ind
		
	addi $t5, $t5, 1			# colCounter++
	addi $t3, $t3, 1			# next char of the line
	j Col_Loop					# go to next column

	Next_Row:
	addi $t4, $t4, 1			# rowCounter++
	j Row_Loop					# go to next row

	Break:
	jr $ra						# jump back to the calling code

printGrid:						# function to print the main grid at the current step
	lw $t0, Grid				# Main Grid --> t0-reg
	lw $t1, Row					# Row --> t1-reg
	lw $t2, Col					# Col --> t2-reg
	
	li $a0, 10					# a0-reg --> new-line ASCII value
	li $v0, 11					# print new-line for readability
	syscall
	
	add $t3, $zero, $zero		# (rowCounter = 0) --> t3-reg
	
	Row_Loop_2:
	beq $t3, $t1, Break_2		# while (rowCounter != Row)
	
	add $t4, $zero, $zero		# (colCounter = 0) --> t4-reg
	
	Col_Loop_2:
	beq $t4, $t2, Next_Row_2	# while (colCounter != Col)
								# calculate index: index = (rowCounter * Col) + colCounter
	mul $t5, $t3, $t2			# t5-reg --> rowCounter * Col
	add $t5, $t5, $t4			# t5-reg --> (rowCounter * Col) + colCounter
	mul $t5, $t5, DATA_SIZE		# t5-reg --> index * DATA_SIZE
	add $t5, $t5, $t0			# t5-reg --> addr in memory
	
	lb $a0, 0($t5)				# a0-reg --> char at the current index	
	li $v0, 11					# print char
	syscall
								
	li $a0, 32					# a0-reg --> space ASCII value
	li $v0, 11					# print space for readability
	syscall
	
	addi $t4, $t4, 1			# colCounter++
	j Col_Loop_2				# go to next column
	
	Next_Row_2:					
	li $a0, 10					# a0-reg --> new-line ASCII value
	li $v0, 11					# print new-line for readability
	syscall
	
	addi $t3, $t3, 1			# rowCounter++
	j Row_Loop_2				# go to next row
	
	Break_2:
	jr $ra						# jump back to the calling code

firstStep:						# function to create a memory grid to hold the copy of a main grid
	lw $t0, Row 				# Row --> t0-reg
	lw $t1, Col					# Col --> t1-reg
	lw $t2, Grid				# Main Grid --> t2-reg
								# dynamically allocating mem for the matrix
	mul $a0, $t0, $t1 			# total num of elements = row * column
	mul $a0, $a0, DATA_SIZE 	# total size = (num of elements) * DATA_SIZE
	li $v0, 9 					# heap allocation
	syscall
	sw $v0, MemGrid 			# allocated mem addr --> MemGrid					
								
	add $t3, $zero, $zero		# (rowCounter = 0) --> t3-reg
	
	Row_Loop_3:						
	beq $t3, $t0, Break_3		# while (rowCounter != Row)
	
	add $t4, $zero, $zero		# (colCounter = 0) --> t4-reg
	
	Col_Loop_3:						
	beq $t4, $t1, Next_Row_3	# while (colCounter != Col)
								# calculate index: index = (rowCounter * Col) + colCounter
	mul $t5, $t3, $t1			# t5-reg --> rowCounter * Col
	add $t5, $t5, $t4			# t5-reg --> (rowCounter * Col) + colCounter
	mul $t5, $t5, DATA_SIZE		# t5-reg --> index * DATA_SIZE
	
	add $t6, $t5, $t2			# t6-reg --> curr ind of main grid
	add $t5, $t5, $v0			# t5-reg --> curr ind of memory grid
								# copying the main grid into the memory grid
	lb $t7, 0($t6)				# t7-reg --> char at curr ind of main grid
	sb $t7, 0($t5)				# t5-reg --> 0(t7-reg): char at curr ind

	addi $t4, $t4, 1			# colCounter++
	j Col_Loop_3				# go to next column

	Next_Row_3:
	addi $t3, $t3, 1			# rowCounter++
	j Row_Loop_3				# go to next row

	Break_3:
	jr $ra						# jump back to the calling code
	
secondStep:						# function to fill the main grid w/ bombs
	lw $t0, Grid 				# Row --> t0-reg
	lw $t1, Row					# Col --> t1-reg
	lw $t2, Col					# Main Grid --> t2-reg
	
	add $t3, $zero, $zero		# (rowCounter = 0) --> t3-reg
	
	Row_Loop_4:
	beq $t3, $t1, Break_4		# while (rowCounter != Row)
	
	add $t4, $zero, $zero		# (colCounter = 0) --> t4-reg
	
	Col_Loop_4:
	beq $t4, $t2, Next_Row_4	# while (colCounter != Col)
								# calculate index: index = (rowCounter * Col) + colCounter
	mul $t5, $t3, $t2			# t5-reg --> rowCounter * Col
	add $t5, $t5, $t4			# t5-reg --> (rowCounter * Col) + colCounter
	mul $t5, $t5, DATA_SIZE		# t5-reg --> index * DATA_SIZE
	add $t5, $t5, $t0			# t5-reg --> addr in memory
	
	addi $t4, $t4, 1			# colCounter++
	
	lb $t6, 0($t5)				# t6-reg --> char at the curr ind	
	bne $t6, '.', Col_Loop_4	# if (char at curr ind != bomb)
	
	li $t6, 'O'					# t6-reg: bomb
	sb $t6, 0($t5)				# char at curr ind = t6-reg (planting bomb)
	
	j Col_Loop_4				# go to next char of the line
	
	Next_Row_4:					
	addi $t3, $t3, 1			# rowCounter++
	j Row_Loop_4				# go to next row
	
	Break_4:
	jr $ra						# jump back to the calling code
	
thirdStep:						# function to destroy the bombs planted 3 sec ago & in the neighbor cells
	lw $t0, Grid 				# Grid --> t0-reg
	lw $t1, MemGrid				# MemGrid --> t1-reg
	lw $t2, Row					# Row --> t2-reg
	lw $t3, Col					# Col --> t3-reg
	
	add $t4, $zero, $zero		# (rowCounter = 0) --> t4-reg
	
	Row_Loop_5:
	beq $t4, $t2, Break_5		# while (rowCounter != Row)
	
	add $t5, $zero, $zero		# (colCounter = 0) --> t5-reg
	
	Col_Loop_5:
	beq $t5, $t3, Next_Row_5	# while (colCounter != Col)
								# calculate index: index = (rowCounter * Col) + colCounter
	mul $t6, $t4, $t3			# t5-reg --> rowCounter * Col
	add $t6, $t6, $t5			# t5-reg --> (rowCounter * Col) + colCounter
	mul $t6, $t6, DATA_SIZE		# t5-reg --> index * DATA_SIZE
	
	add $t7, $t6, $t0			# t7-reg --> curr ind of main grid
	add $t6, $t6, $t1			# t6-reg --> curr ind of mem grid
	
	lb $t8, 0($t6)				# t8-reg --> char at the curr ind of mem grid
	bne $t8, 'O', Else			# if (char at curr ind of mem grid == bomb)
	
	li $t9, '.'					# t9-reg: empty cell
	sb $t9, 0($t7)				# char at curr ind of main grid = t8-reg (destroying bomb)
	
	If_1:
	beq $t5, 0, If_2			# if (colCounter != 0)
	
	add $t7, $t7, -1			# t7-reg --> prev cell: Grid[rowCounter * Col + colCounter - 1]
	sb $t9, 0($t7)				# t7-reg: emptied
	add $t7, $t7, 1				# t7-reg --> back to curr ind
	
	If_2:
	add $t8, $t3, -1			# t8-reg --> Col - 1 (boundry check)
	beq $t5, $t8, If_3			# if (colCounter != Col - 1)
	
	add $t7, $t7, 1				# t7-reg --> next cell: Grid[rowCounter * Col + colCounter + 1]
	sb $t9, 0($t7)				# t7-reg: emptied
	add $t7, $t7, -1			# t7-reg --> back to curr ind
	
	If_3:
	beq $t4, 0, If_4			# if (rowCounter != 0)
								# calculating index: above index = grid[(rowCounter - 1) * Col + colCounter]
	add $t8, $t4, -1			# t8-reg --> rowCounter - 1
	mul $t8, $t8, $t3			# t8-reg --> (rowCounter - 1) * Col
	add $t8, $t8, $t5			# t8-reg --> (rowCounter - 1) * Col + colCounter
	add $t8, $t8, $t0			# t8-reg --> above ind = ind + addr of grid
	sb $t9, 0($t8)				# emptied
	
	If_4:
	add $t8, $t2, -1			# t8-reg --> Row - 1 (boundry check)
	beq $t4, $t8, Else			# if (rowCounter != Row - 1)
								# calculating index: below index = grid[(rowCounter + 1) * Col + colCounter]
	add $t8, $t4, 1				# t8-reg --> rowCounter + 1
	mul $t8, $t8, $t3			# t8-reg --> (rowCounter + 1) * Col
	add $t8, $t8, $t5			# t8-reg --> (rowCounter + 1) * Col + colCounter
	add $t8, $t8, $t0			# t8-reg --> below ind = ind + addr of grid
	sb $t9, 0($t8)				# emptied
	
	Else:
	addi $t5, $t5, 1			# colCounter++
	j Col_Loop_5				# go to next cell of the column
	
	Next_Row_5:					
	addi $t4, $t4, 1			# rowCounter++
	j Row_Loop_5				# go to next row
	
	Break_5:
	jr $ra						# jump back to the calling code