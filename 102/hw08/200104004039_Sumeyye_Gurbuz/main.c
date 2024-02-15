#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int main(void)
{	int xs, seqlen, *loop, looplen, *h, digit;
	printf("Please enter the sequence length: "); scanf("%d", &seqlen);
	printf("Please enter the first element: "); scanf("%d", &xs);
	h = (int*)calloc(9, sizeof(int));					/* giving array 'h' a size of 9 (number of digits)					*/
	looplen = seqlen/2;							/* initial value of looplen								*/
	check_loop_iterative(generate_sequence, xs, seqlen, loop, &looplen);	/* calling "check_loop_iterative" with its initial values					*/
	if(looplen != 0)							/* if there is a loop - print it								*/
	{	printf("Loop: {");
		for (int i = 0; i < looplen; i++)
		{	if(i == looplen-1) printf("%d}\n", loop[i]);
			else printf("%d, ", loop[i]);
		}
	}
	else printf("\nNo loop found.\n");
	hist_of_firstdigits(generate_sequence, xs, seqlen, h, 1);			/* calling "hist_of_firstdigits" with its initial values					*/
	printf("\nHistogram of the sequence: {");				/* print histogram of the sequence							*/
	for (int i = 0; i < 9; i++)
	{	if(i == 8) printf("%d}\n", h[i]);
		else printf("%d, ", h[i]);
	}
	return 0;
}
