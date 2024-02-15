#include <stdio.h>
#include "utils.h"

int main()
{
	printf("--------------------------------------------------\nPART 1\n--------------------------------------------------\n");
	int x, y, z, n;
	printf("Enter the first integer: ");
	scanf("%d", &x);
	printf("Enter the second integer: ");
	scanf("%d", &y);
	printf("Enter the divisor: ");
	scanf("%d", &z);
	// calling Part 1
	int returned_part1 = part1(x, y, z);
	if(returned_part1 == -1)   // error message
	{
		printf("There is not any integer between %d and %d can be divided by %d.\n", x, y, z);
	}
	
	printf("--------------------------------------------------\nPART 2\n--------------------------------------------------\n");
	int returned_part2 = part2();   // calling Part 2
	if(returned_part2 == 1) printf("\nRegistering cannot be done due to the invalid identity number\n");   // error message
	else   // if customer successfully registered, call Part 3 to login screen
	{
		printf("--------------------------------------------------\nPART 3\n--------------------------------------------------\n");
		int returned_part3 = part3();
		if(returned_part3 == 1) printf("Invalid identity number or password\n");   // error message --> cannot login
		else printf("Withdrawable amount is: %d\n", returned_part3);   // if login is done, print withdrawable amount
	}
	
	return 0;
}
