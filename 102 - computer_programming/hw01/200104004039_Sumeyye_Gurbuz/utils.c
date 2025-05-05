#include <stdio.h>
#include <string.h>
#include "utils.h"

int find_divisible(int x, int y, int z) {
	int f_i;
	for (f_i = x; f_i <= y;)
	{
		if(f_i % z == 0) return f_i;	// if there is an int whose remainder of the divisor is equal zero, function will return that int immediately
		else ++f_i;	
	}
	return -1;	// otherwise function will return an error
}

int find_nth_divisible(int n, int f_i, int z) {
	int nth = f_i + n*z;
	return nth;
}

int part1(int x, int y, int z) {
	if(x - y > 0) return -1;   // if first int > second int, we'll return an error to main
	else
	{
		int f_i = find_divisible(x, y, z);   // finding if there is a first int between the range
		if(f_i == -1) return -1;   // returning error to main
		else
		{
			printf("The first integer between %d and %d divided by %d is %d.\n", x, y, z, f_i);
			// looking for nth divisible number between the range
			int n;
			printf("Enter the number how many next: ");
			scanf("%d", &n);
			int nth = find_nth_divisible(n, f_i, z);   // taking return value
			// we'll check if it is in the bounds
			if(nth > y)
				printf("No possible to find %dth divisible between %d and %d divided by %d.\n", n+1, x, y, z);
			else
				printf("The %dth integer between %d and %d divided by %d is %d.\n", n+1, x, y, z, nth);
		}
	}
}

int validate_identity_number(char identity_number[]) {
	if(strlen(identity_number) != 11) return 1;   // if size of ID is not 11, return invalid to main
	else
	{
		if(identity_number[0] - 48 == 0) return 1;   // if first character of ID is 0, return invalid to main
		else
		{
			for (int i = 0; i < 11; i++)
			{	// if any char of ID is not a digit, return invalid to main
				if(identity_number[i] - 48 < 0 || identity_number[i] - 48 > 9) return 1;
			}
		}
		int first_Sum = 0, second_Sum = 0;
		// sum of even digits
		for (int i = 1; i < 9; i += 2)
		{
			first_Sum += identity_number[i] - 48;
		}
		// sum of odd digits
		for (int i = 0; i < 10; i += 2)
		{
			second_Sum += identity_number[i] - 48;
		}
		second_Sum *= 7;   // 7 times sum of odd digits
		
		// determining which sum is bigger than the other, we'll make calculations according to this result
		int Max = first_Sum, Min = first_Sum;
		if(first_Sum - second_Sum <= 0)
		{
			Max = second_Sum;
		}
		else
		{
			Min = second_Sum;
		}
		int result = (Max - Min) % 10;   // subtracting the sums and finding the remainder of division by 10
		if(result != identity_number[9]-48) return 1;   // if remainder is not equal to 10th digit, return invalid to main
		else
		{
			int total_Sum = 0;
			for (int i = 0; i < 10; i++)
			{	// calculating the total sum of ID (except 11th digit)
				total_Sum += identity_number[i] - 48;
			}
			if(total_Sum % 10 != identity_number[10]-48) return 1;   // if MOD10 of total sum not equal to 11th digit, return invalid to main
			else return 0;   // if ID is valid, return valid to part2()
		}
	}
}

int create_customer(char identity_number[], int password) {
	int saved_password[4];
	// we'll find every digit of password and save them into an int array, this'll make reading easier after saving it in a file
	for (int i = 3; i >= 0; i--)
	{
		saved_password[i] = password % 10;
		password /= 10;
	}
	FILE *accounts;
	accounts = fopen("customeraccount.txt", "w");
	// writing data to the file
	fputs(identity_number, accounts);
	fputs(",", accounts);
	for (int i = 0; i < 4; i++) fprintf(accounts, "%d", saved_password[i]);

	fclose(accounts);
	
	printf("\nRegistering done successfully\n");
}	
	
int part2() {
	char identity_number[10000];   // we'll declare an infinite size array, so that even if user wants to enter an id size of 50, we'll be able to return an error
	printf("Please enter your identity number to register: ");
	scanf("%s", identity_number);
	int password;
	printf("Please enter a 4 digit password to register (exp: 1234) : ");
	scanf("%d", &password);

	int returned = validate_identity_number(identity_number);
	if(returned == 0)   // if ID is valid, call create_customer()
		create_customer(identity_number, password);
		
	return returned;
}

int check_login(char identity_number[], int password) {
	// again we'll find the digits of login password to compare with original passwd
	int login_password[4];
	for (int i = 3; i >= 0; i--)
	{
		login_password[i] = password % 10;
		password /= 10;
	}
	
	FILE *accounts;
	accounts = fopen("customeraccount.txt", "r");
	// reading registering data to comparison
	char saved_id[11];
	int saved_password[4];
	for (int i = 0; i < 11; i++) saved_id[i] = fgetc(accounts);
	fgetc(accounts);
	for (int i = 0; i < 4; i++) saved_password[i] = fgetc(accounts)-48;
	
	fclose(accounts);
	
	for (int i = 0; i < 11; i++)
	{	// if IDs won't match, return cannot login
		if(saved_id[i] != identity_number[i]) return 1;
		else
		{
			for (int i = 0; i < 4; i++)
			{	// if PASSWDs won't match, return cannot login
				if(saved_password[i] != login_password[i]) return 1;
			}
		}
	}
	
	return 0;   // if login data is true, return 0
}

int withdrawable_amount(float cash_amount) {	
	/* find division of cash_amount by 10 and multiply it by ten to find withdrawable amount:
	
		$ cash_amount : 817.23
		
		$ division by 10 : 81
		$ multiplication by 10 : 810 
	*/
	int amount = cash_amount / 10.0;
	amount *= 10;
	return amount;
}

int part3() {	
	char identity_number[12];
	identity_number[11] = '\0';   // we'll declare this to avoid any error
	int password;
	printf("Please enter your ID to login: ");
	scanf("%s", identity_number);
	printf("Please enter your password: ");
	scanf("%d", &password);
	// sending login data to check_login function
	int returned = check_login(identity_number, password);
	if(returned == 0)   // if successfully logined, call withdraw function
	{
		float cash_amount;
		printf("\nLogin successful\n\n");
		printf("Enter your withdraw amount: ");
		scanf("%f", &cash_amount);
		// return withdrawble amount
		returned = withdrawable_amount(cash_amount);
	}
	return returned;
}
