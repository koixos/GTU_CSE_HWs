#include <stdio.h>
#include "util.h"

void generate_sequence(int xs, int currentlen, int seqlen, int *seq)
{	if(currentlen < seqlen)								/* we'll generate a sequence by using recursive as a for loop			*/
	{	seq[currentlen] = xs;							/* make "currentlen"th index of array 'seq' current number			*/
		if(xs % 2 == 0) generate_sequence(xs/2, currentlen+1, seqlen, seq);
		else generate_sequence(xs*3+1, currentlen+1, seqlen, seq);
	}
} /* END of generate_sequence */
int has_loop(int *arr, int n, int looplen, int *ls, int *le)
{	int j, flag = 0, for_loop[looplen], temp[looplen];				/* we'll use two loops:								*/
	printf("Checking if there is a loop of length %d...\n", looplen);			/* one for holding actual array, other for holding a copy of first for comparison	*/
	for (int i = 0; i < n; i++)							/* n: seqlen									*/
	{	for (j = 0; j < looplen; j++)						/* In this part, we'll look for any loop.					*/
		{	for_loop[j] = arr[j+i];						/* First actual array (size of looplen) will be filled with indexed numbers of array */
			temp[j] = arr[j+i+looplen];					/* Then, copy array will be filled with num.s which are "looplength" more than previous*/
		}
		for (j = 0; j < looplen; j++)
			if(for_loop[j] != temp[j]) flag = 1;				/* If these two arrays are not similar (if there is no loop), then flag will be 1.	*/
		if(flag == 0)								/* If flag is still 0, meaning there is a loop					*/
		{	*ls = i; *le = j+i;						/* "ls" and "le" values will be defined.						*/
			return looplen;							/* And looplength will be returned as itself to use in "main" function.		*/
		}
		else flag = 0;								/* If flag is changed, we should reset it to use again.				*/
	}
	return looplen = 0;								/* If all sequence's searched and there isnot a loop, then looplen will return as 0. */
} /* END of has_loop */
void check_loop_iterative(void (*f)(int, int, int, int*), int xs, int seqlen, int *loop, int *looplen)
{	int i, seq[seqlen], ls, le, temp;
	if(*looplen >= 1)								/* We'll use number "1" as a control flag.					*/
	{	generate_sequence(xs, 0, seqlen, seq);					
		if(*looplen == seqlen/2)						/* This checking is needed to print sequence only once.				*/
		{	printf("\nSequence: {");
			for (i = 0; i < seqlen; i++)
			{	if(i == seqlen-1) printf("%d}\n\n", seq[i]);
				else printf("%d, ", seq[i]);
			}
		}
		if(*looplen != 1)							
		{	temp = has_loop(seq, seqlen, *looplen, &ls, &le);			
			if(temp != 0)							/* "temp" is not 0 means that there is a loop					*/
			{	printf("\nLoop detected with a length of %d.\nThe indexes of the loop's first occurance: %d (first digit), %d (last digit)\n", *looplen,ls,le-1);
				for (i = ls; i < le; i++)				/* Here we're filling the array "loop" with related indices of array "seq".	*/
					loop[i-ls] = seq[i];
			}
			else if(temp == 0 && *looplen >= 2)				/* this statement means "if there is no loop AND looplength doesnot become "1" yet" */
			{	--*looplen;						/* decrease looplength by 1 and call the recursive				*/
				check_loop_iterative(generate_sequence, xs, seqlen, loop, looplen);
			}
		}
		else *looplen = 0;							/* if loop-length becomes "1", then "check_loop_iterative" is done.		*/
	}
} /* END of check_loop_iterative */
void hist_of_firstdigits(void (*f)(int, int, int, int*), int xs, int seqlen, int *h, int digit)
{	int seq[seqlen];
	if(digit < 10)									/* digit range: 1-9								*/
	{	generate_sequence(xs, 0, seqlen, seq);
		for (int i = 0; i < seqlen; i++)
		{	while(seq[i] >= 10) seq[i] /= 10;				/* calculating first digits of numbers of array "seq"				*/
			if(seq[i] == digit) h[seq[i]-1] += 1;				/* if first digit of seq[i] is equal current digit value,				*/
		}									/* ... then increase that indice of array "h" by 1				*/
		hist_of_firstdigits(generate_sequence, xs, seqlen, h, digit+1);		/* call function again until digit == 10						*/
	}
} /* END of hist_of_firstdigits */
