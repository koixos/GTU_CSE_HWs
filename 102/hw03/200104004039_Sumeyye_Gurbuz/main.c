#include <stdio.h>
#include <math.h>

int display_menu(void)
{
	int selection;
	printf("----------------------------\nSelect an operation:\n\n  1. Calculate sum/multiplication between two numbers\n  2. Calculate prime numbers\n  3. Show number sequence in file\n  4. Sort number sequence in file\n  0. Exit\n------------------------\n");
	printf("Selection: ");
	scanf("%d", &selection);
	printf("---------------------\n");
	return selection;
}
void write_file(int number)
{
	FILE *ptr;
	ptr = fopen("results.txt", "aw"); // opening file in append+write mode
	
	fprintf(ptr, "%d ", number);
	printf("\nThe result is written to \"results.txt\" file.\n");	
	
	fclose(ptr);
}
void print_file()
{
	FILE *ptr;
	ptr = fopen("results.txt", "r");
	
	int num;
	printf("Result:\n");
	while (fscanf(ptr, "%d ", &num) == 1) // if there is a number in the file,
		printf("%d ", num); // print it on screen
	printf("\n");
	fclose(ptr);
}
void print_sorted_file(void)
{	
	FILE *ptr, *ptr2;
	ptr = fopen("temp.txt", "r"); // While reading numbers in temp.txt, 
	ptr2 = fopen("results.txt", "w"); // I'll write them into results.txt
	
	int number;
	while (fscanf(ptr, "%d ", &number) == 1)
		fprintf(ptr2, "%d ", number);
	fclose(ptr2);
	fclose(ptr);
	
	printf("Sorting completed.\n\n");
	print_file(); // calling printf_file to print them on terminal
}
int sorting_three_number(long int min3) // I'll use min3 as the minimum value and compare it with other numbers to be able to find min1 values.
{	// after every reading, we should start the file again; otherwise we cannot read it from the start but the end
	FILE *ptr;
	// why "2147483648" ? Because the max int value is 2147483647.
	long int number, min1 = 2147483648, min2 = 2147483648; // initialization before finding min values
	ptr = fopen("results.txt", "r");
	while (fscanf(ptr, "%ld ", &number) == 1)
		if(number > min3 && number < min1) // If number is bigger than parameter min3 but smaller than its initialized value, then it'll be the new min1
			min1 = number;
	fclose(ptr);
	
	ptr = fopen("results.txt", "r");
	while (fscanf(ptr, "%ld ", &number) == 1)
		if(number > min1 && number < min2) // If number is bigger than min1 but smaller than its initialized value, then it'll be the new min2
			min2 = number;
	fclose(ptr);
	
	ptr = fopen("results.txt", "r");
	min3 = 2147483648; // cleaning min3 to find new min3
	while (fscanf(ptr, "%ld ", &number) == 1)
		if(number > min2 && number < min3) // If number is bigger than min2 but smaller than its initialized value, then it'll be the new min3
			min3 = number;
	fclose(ptr);

	ptr = fopen("temp.txt", "aw"); // opening temp file in append+write mode
	// I'll make this comparisons to avoid any mistake. If min1, min2 or min3 equals 2147483648, I'll not take them in temp file since this means overflow
	if(min1 != 2147483648 && min2 != 2147483648 && min3 == 2147483648)
		fprintf(ptr, "%ld %ld ", min1, min2);
	else if(min1 != 2147483648 && min2 == 2147483648 && min3 == 2147483648)
		fprintf(ptr, "%ld ", min1);
	else if(min1 == 2147483648 && min2 == 2147483648 && min3 == 2147483648)
		fprintf(ptr, " ");
	else
		fprintf(ptr, "%ld %ld %ld ", min1, min2, min3);
	fclose(ptr);
	
	return min3; // returning min3 to the main to be able to use it
}
void sort_file()
{	// after every reading, we should start the file again; otherwise we cannot read it from the start but the end
	FILE *ptr;
	ptr = fopen("results.txt", "r");
	
	long int counter = 0, number;
	while (fscanf(ptr, "%ld ", &number) == 1) // I'll find that there is how many numbers in results.txt
		++counter;
	fclose(ptr);
	
	ptr = fopen("results.txt", "r");
	long int min1 = 2147483648, min2 = 2147483648, min3 = 2147483648;
	while (fscanf(ptr, "%ld ", &number) == 1)
		if(number < min1) // determining the FIRST min1 value
			min1 = number;
	fclose(ptr);
	
	ptr = fopen("results.txt", "r");
	while (fscanf(ptr, "%ld ", &number) == 1)
		if(number > min1 && number < min2) // determining the FIRST min2 value
			min2 = number;
	fclose(ptr);
	
	ptr = fopen("results.txt", "r");
	while (fscanf(ptr, "%ld ", &number) == 1)
		if(number > min2 && number < min3) // determining the FIRST min3 value
			min3 = number;
	fclose(ptr);
	// write these first values into temp.txt
	ptr = fopen("temp.txt", "w");
	fprintf(ptr, "%ld %ld %ld ", min1, min2, min3);
	fclose(ptr);
	// now I'll determine the other numbers' order, and for that min3 value will be used as the threshold
	if(counter % 3 == 0) // if mod of counter is equal to zero, then call sorting function for number of division times - 1...Why n-1? 'cause I've already sorted it once above
		for (int i = 1; i < counter/3; i++) // exp: 15 % 3 = 0, 15 / 3 = 5...call func. for 4 times
			min3 = sorting_three_number(min3);
	else // if mod is not 0, then call function for number of division times which means call it for n+1 time to be able to sort remained numbers too
		for (int i = 0; i < counter/3; i++) // exp: 14 % 3 = 2, 14 / 3 = 4...call funct. for 5 times, actually what I did is rounding 14 to 15
			min3 = sorting_three_number(min3);
			
	print_sorted_file();
}
int sum(int n1, int n2, int flag)
{
	int result = 0;
	printf("\nResult:\n");
	switch(flag)
	{	// if even
		case 0:
			for (int i = n2+1; i < n1; i++)
			{
				if(i % 2 == 0)
				{
					result += i;
					if(i+2 == n1 || i+1 == n1) // if it is the last number
						printf("%d = ", i); // print only "="
					else			  // if it is not
						printf("%d + ", i); // print "+"
				}
			}
			printf("%d\n", result);
			break;
		// if odd	
		case 1:
			for (int i = n2+1; i < n1; i++)
			{
				if(i % 2 != 0)
				{
					result += i;
					if(i+2 == n1 || i+1 == n1)
						printf("%d = ", i);
					else
						printf("%d + ", i);
				}
			}
			printf("%d\n", result);
			break;
	}
	return result;
}
int multi(int n1, int n2, int flag)
{
	int result = 1;
	printf("\nResult:\n");
	switch(flag)
	{	// if even
		case 0:
			for (int i = n2+1; i < n1; i++)
			{
				if(i % 2 == 0)
				{
					result *= i;
					if(i+2 == n1 || i+1 == n1)
						printf("%d = ", i);
					else
						printf("%d * ", i);
				}
			}
			printf("%d\n", result);
			break;
		// if odd
		case 1:
			for (int i = n2+1; i < n1; i++)
			{
				if(i % 2 != 0)
				{
					result *= i;
					if(i+2 == n1 || i+1 == n1)
						printf("%d = ", i);
					else
						printf("%d * ", i);
				}
			}
			printf("%d\n", result);
			break;
	}
	return result;
}
int isprime(int a)
{
	int flag = 0;
	for (int divisor = 2; divisor <= sqrt(a); divisor++)
	{	// if it is not prime, return its least divisor
		if(a % divisor == 0)
			return divisor;
	}
	return flag; // if it is prime, return "it is prime" flag
}
int main()
{
	int exit = 0;
	while(exit == 0)
	{
		// calling menu
		int selection = display_menu();
		if(selection == 1)
		{
			int exit = 1, first_flag, sec_flag, num1, num2;
			while (exit != 0)
			{
				printf("  0. Addition\n  1. Multiplication\n\n  Operation: ");
				scanf("%d", &first_flag);
				// if it is a valid selection, quit while
				if(first_flag == 0 || first_flag == 1)
					exit = 0;
				else
					printf("\nInvalid operation. Try again.\n-----------------------------------\n");
			}
			exit = 1; // reseting exit to use again
			while (exit != 0)
			{
				printf("------------------------------\n  0. Even numbers\n  1. Odd numbers\n\n  Selection: ");
				scanf("%d", &sec_flag);
				// if it is a valid selection, quit while
				if(sec_flag == 0 || sec_flag == 1)
					exit = 0;
				else
					printf("\nInvalid selection. Try again.\n");
			}
			printf("------------------------------\nPlease enter two different numbers: \n");
			printf("Number 1: ");
			scanf("%d", &num1);
			printf("Number 2: ");
			scanf("%d", &num2);
			
			// determining max & min number
			int max = num1, min = num1;
			if(num1 - num2 >= 0) min = num2;
			else max = num2;
			
			int result;
			switch(first_flag)
			{	// if addition
				case 0:
					result = sum(max, min, sec_flag);
					write_file(result);
					break;
				// if multiplication
				case 1:
					result = multi(max, min, sec_flag);
					write_file(result);
					break;
			}
		}
		else if(selection == 2)
		{
			int num;
			printf("Please enter an integer: ");
			scanf("%d", &num);
			
			for (int i = 2; i < num; i++)
			{
				int returned = isprime(i);
				if(returned == 0)
					printf("%d is a prime number.\n", i);
				else
					printf("%d is not a prime number, it is divisible by %d.\n", i, returned);
			}
		}
		else if(selection == 3) print_file();
		else if(selection == 4) sort_file();
		else if(selection == 0)
		{
			printf("Exitting...\n");
			exit = 1;
		}
		else printf("Invalid selection\n");
	}
	return 0;
}
