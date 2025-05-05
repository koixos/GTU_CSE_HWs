/*
										SOLUTION FOR TOWER OF HANOI PROBLEM
	
	- After making some tests and trials-and-errors to understand the logic behind this problem, I have found a pattern between next move and the number of blocks on a rod.
	- The pattern to set an algorithm:
		
		# start with 1st rod -
		# [ size = first->currentsize ]  
			> if mod(size, 2) is different from 0, then move the top block to 3rd rod
			> if mod(size, 2) is 0, move the top block to 2nd rod
			> check 1st rod until there is no possible popping - if there isnot remained any possible move on 1st rod,
		
			# pass to 2nd rod - 
			# [ size = second->currentsize ] 
				> if mod(size, 2) is different from 0, move the top block to 3rd rod
				> if mod(size, 2) is 0, move the top block to 1st rod
				> check 2nd rod till there is no possible moving - if there isnot any remained move on 2nd rod,
		
				# pass to 3rd rod -
				# [ size = third->currentsize ]
					> if mod(size, 2) is different from 0, move the top block to 2nd rod
					> if mod(size, 2) is 0, move the top block to 1st rod
					> check 3rd rod till there is no possible moving - if there isnot any remained move on 3rd rod,
					
					# go back to first rod and repeat this 'till the problem is done
	
	- We have to check some conditions throughout this algorithm to be able to solve the problem in minimum move.
	- Minimum number of move for any size is determined with this formula: [ total_movement = 2^size - 1 ]
	- These conditions should be checked on every move:
		
		$ whether destination->array[top] is bigger than the block wanted to move - if not, the block cannot be moved - top of destination should be bigger than top of source
		$ number of moves should be decreased by 1 with every move
		$ size of source rod should be bigger than 0 - meaning it cannot be empty
		$ if destination rod is empty any move can be made
		$ the latest moved block should be saved to a variable to avoid creating any kind of infinite loop,
			exp:
			-> block 1 is moved to 3rd rod from 1st rod,
			-> If there is no move on 2nd rod, the program will flow normally. But if we did not save the latest moved block, when the program comes to 3rd rod,
				it'll forget that the block 1 has been moved there in the previous move and it may try to move it to 1st rod again (if conditions are satisfied),
				and this will cause a probable loop.
			-> So we have to save the latest move to use it as a memory.
		
		$ if top of next source is equal to memory block which is saved from the previous move, there is no move as explained above
*/
#include <stdio.h>
#include <stdlib.h>
#define STACK_BLOCK_SIZE 10
#define TRUE 1

typedef struct {
	int *array;
	int currentsize;
	int maxsize;
}stack;

int push(stack *s, int d);										/*	The stack array will grow STACK_BLOCK_SIZE entries at a time				*/

int pop(stack *s); 											/*	The stack array will shrink STACK_BLOCK_SIZE entries at a time			*/

stack *init_return();											/*	Initializes an empty stack				     				*/

int init(stack *s); 											/*	Returns "1" if initialization is successful 		     				*/

stack *expand(int d);											/*	Function to avoid stack overflow							*/

int move_block(stack *source, stack *destination);							/*	Function to move a block from source to destination					*/

int main()
{	int i, size, remainedmove = 1, memory = 0;							/*	"remainmove"	: holds number of remained move
														"memory"	: holds the block moved last						*/
	stack	*first	= init_return(),
		*second = init_return(),
		*third	= init_return();
		
	printf("Enter tower size: "); scanf("%d", &size); printf("\n");
	if(size > 10)											/*	If given size is bigger than STACK_BLOCK_SIZE, we'll expand each stack by that size	*/
	{	first	= expand(size);
		second	= expand(size);
		third	= expand(size);
	}
	
	for (i = 0; i < size; i++)									/*	Formula for total number of move: [ 2^(size) - 1 ]					*/
		remainedmove *= 2;
	remainedmove -= 1;
	
	for (i = 0; i < size; i++)									/*	Filling first rod with blocks				     				*/
		push(first, size - i);									/*	Biggest block at the bottom and smallest block at the top				*/
	
	/* ALGORITHM IS EXPLAINED ABOVE */
	while (remainedmove)
	{	while (TRUE)
		{	if(first->currentsize > 0 && first->array[first->currentsize - 1] != memory)
			{	if(first->currentsize % 2 != 0 && (third->array[third->currentsize - 1] == 0 || third->array[third->currentsize - 1] > first->array[first->currentsize - 1]))
				{	memory = move_block(first, third);
					printf("Move the disk %d from '1' to '3'\n", memory);	
				}
				else if(first->currentsize % 2 == 0 &&(second->array[second->currentsize - 1] == 0 || second->array[second->currentsize - 1] > first->array[first->currentsize - 1]))
				{	memory = move_block(first, second);
					printf("Move the disk %d from '1' to '2'\n", memory);	
				} 
				else break;
				--remainedmove;
			}
			else break;									/*	If there is no possible move, pass the second rod					*/
		}
		while (TRUE)
		{	if(second->currentsize > 0 && second->array[second->currentsize - 1] != memory)
			{	if(second->currentsize % 2 != 0 && (third->array[third->currentsize - 1] == 0 || third->array[third->currentsize - 1] > second->array[second->currentsize - 1]))
				{	memory = move_block(second, third);
					printf("Move the disk %d from '2' to '3'\n", memory);	
				}
				else if(second->currentsize % 2 == 0 && (first->array[first->currentsize - 1] == 0 || first->array[first->currentsize - 1] > second->array[second->currentsize - 1]))
				{	memory = move_block(second, first);
					printf("Move the disk %d from '2' to '1'\n", memory);	
				}
				else break;
				--remainedmove;
			}
			else break;									/*	If there is no possible move, pass the third rod					*/
		}
		while (TRUE)
		{	if(third->currentsize > 0 && third->array[third->currentsize - 1] != memory)
			{	if(third->currentsize % 2 != 0 && (second->array[second->currentsize - 1] == 0 || second->array[second->currentsize - 1] > third->array[third->currentsize - 1]))
				{	memory = move_block(third, second);
					printf("Move the disk %d from '3' to '2'\n", memory);	
				}
				else if(third->currentsize % 2 == 0 && (first->array[first->currentsize - 1] == 0 || first->array[first->currentsize - 1] > third->array[third->currentsize - 1]))
				{	memory = move_block(third, first);
					printf("Move the disk %d from '3' to '1'\n", memory);	
				}
				else break;
				--remainedmove;
			}
			else break;									/*	If there is no possible move, pass the first rod					*/
		}
	}
	return 0;
} /* END of main */
int push(stack *s, int d)
{	s->array[s->currentsize] = d;									/*	"d" is made the latest element of stack array	     					*/
	++s->currentsize;										/*	To avoid overflow and expand the array "currentsize" is increased by 1		*/
} /* END of push */
int pop(stack *s)
{	s->array[s->currentsize - 1] = '\0';								/*	To shrink the stack, the latest element of the stack array is made NULL		*/
	--s->currentsize;										/*	Then "currentsize" is decreased by 1							*/
} /* END of pop */
stack *init_return()
{	stack *s = (stack*)malloc(sizeof(stack));

	s->maxsize	= STACK_BLOCK_SIZE;								/*	Stack will start initially with a size of STACK_BLOCK_SIZE				*/
	s->currentsize	= 0;										/*	Index of top element for beginning							*/
	s->array	= (int*)malloc(STACK_BLOCK_SIZE * sizeof(int));			
	
	if(init(s) == 1) return s;									/*	If stack is initialized successfully return it						*/
} /* END of init_return */
int init(stack *s)
{	if(s != NULL) return 1;									/*	If initialization of stack is done return 1						*/
} /* END of init */
stack *expand(int d)
{	stack *s = (stack*)malloc(sizeof(stack));							/*	This function will be called if the given size transcends STACK_BLOCK_SIZE,		*/

	s->maxsize	= STACK_BLOCK_SIZE + d;							/*	and expand the stack by the given size.						*/
	s->currentsize	= 0;
	s->array	= (int*)malloc((STACK_BLOCK_SIZE + d) * sizeof(int));				/*	Finally "maxsize" = given size + STACK_BLOCK_SIZE					*/

	if(init(s) == 1) return s;
} /* END of expand */
int move_block(stack *source, stack *destination)							/*	A function to move a block between two rod						*/
{	push(destination, source->array[source->currentsize - 1]);					/*	First the value will be pushed to destination-rod,					*/
	pop(source);											/*	then it will be popped from source-rod;						*/
	return destination->array[destination->currentsize - 1];					/*	and the number of moved block will return to be used as memory			*/
} /* END of move_block */
