#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUST 50
#define MAX_LOAN 3
// amount = 996.3484, interestRate = 0.230075505;
typedef union {
	char name[100];
	char address[100];
	long int phone;
}Person;
typedef struct {	
	float amount;
	float interestRate;
	int period;
}Loan;
typedef struct {
	Person Customer;
	Loan Loans[MAX_LOAN];
}BankAccount;

void displayMenu(char *selection);
void listCustomers(BankAccount *members, int num_of_custs);
void addCustomer(BankAccount *members);
float newLoan(BankAccount *members, int member_id, int total_loans);
float calculateLoan(float amount, int period, float interestRate, float loan);
void getReport(int selection, int member_id);

int main()
{	int i, exit = 0, counter = 1, num_of_loans[MAX_CUST];
	float taken[MAX_CUST][MAX_LOAN];
	char *selection;
	BankAccount *members = (BankAccount*)calloc(MAX_CUST, sizeof(BankAccount));
	for (i = 0; i < MAX_CUST; i++)
		num_of_loans[i] = 0;
	while (exit == 0)
	{	int sum = -48;
		selection = (char*)calloc(50, sizeof(char));
		displayMenu(selection);
		for (i = 0; i < 50; i++)
			sum += selection[i];
			
		if(sum == 1) listCustomers(members, counter);
		else if(sum == 2)
		{	if(counter <= MAX_CUST)
			{	FILE *ptr = fopen("customers.txt", "aw");
				fprintf(ptr, "Customer ID: %d\n", counter);
				fclose(ptr);
				addCustomer(members+counter);
				++counter;
			}
			else printf("System is full! New customers cannot be added anymore.\n");
		}
		else if(sum == 3)
		{	char cust_name[50];
			int flag = 0;
			printf("Enter the borrower customer name: "); scanf(" %[^\n]%*c", cust_name);
			for (i = 0; i < counter; i++)
			{	if(strcmp(cust_name, members[i].Customer.name) == 0)
				{	flag = i;
					break;
				}
			}
			if(flag == 0) printf("Customer \"%s\" cannot be found.\n", cust_name);
			else
			{	if(num_of_loans[flag-1] < 3)
				{	int total_loans = num_of_loans[flag];
					taken[flag-1][total_loans] = newLoan(members, flag, total_loans);
					++num_of_loans[flag-1];
				}
				else printf("Any customer can get a maximum of three loans.\n");
			}
		}
		else if(sum == 4)
		{	int report_section, flag = 0;
			char cust_name[50];
			printf("1. Customer Detail\n2. Loan Detail\nSelection: "); scanf("%d", &report_section);
			printf("Enter the customer's name: "); scanf("%s", cust_name);
			for (i = 0; i < counter; i++)
			{	if(strcmp((members+i)->Customer.name, cust_name) == 0)
					flag = i;
			}
			if(report_section != 1 && report_section != 2)
				printf("Invalid selection.\n");
			else if(flag == 0)
				printf("Customer \"%s\" cannot be found.\n", cust_name);
			else getReport(report_section, flag);
		}
		else if(sum == 5) { printf("Exitting...\n"); exit = 1; }
		else
		{	getchar();
			printf("Invalid selection! Please try again -\n");
		}
		free(selection);
	}
	return 0;
}
void displayMenu(char *selection)
{	printf("==========================================================================\n Welcome to Bank Management System\n==========================================================================\n\t1. List All Customers\n\t2. Add New Customers\n\t3. New Loan Application\n\t4. Report Menu\n\t5. Exit\n---------------------------------------------------------------------------\nSelection: ");
	scanf("%s", selection);
	printf("---------------------------------------------------------------------------\n");
} /* END of displayMenu*/
void listCustomers(BankAccount *members, int num_of_custs)
{	for (int i = 1; i < num_of_custs; i++) printf("%d. %s\n", i, members[i].Customer.name);
} /* END of listCustomers */
void addCustomer(BankAccount *members)
{	long int phone;
	char address[100];
	FILE *ptr = fopen("customers.txt", "aw");
	printf("Enter the name: "); scanf(" %[^\n]%*c", members->Customer.name);
	printf("Enter the phone number: "); scanf("%ld", &phone);
	printf("Enter the address: "); scanf(" %[^\n]%*c", address);
	fprintf(ptr, "Customer Name: %s\nCustomer Phone: %ld\nCustomer Address: %s\n", members->Customer.name, phone, address);
	fclose(ptr);
} /* END of addCustomer */
float newLoan(BankAccount *members, int member_id, int total_loans)
{	float loan = 1.0;
	printf("Enter the amount of loan: "); scanf("%f", &members[member_id].Loans[total_loans].amount);
	printf("Enter the interest rate: "); scanf("%f", &members[member_id].Loans[total_loans].interestRate);
	printf("Enter the period of loan: "); scanf("%d", &members[member_id].Loans[total_loans].period);
	loan = calculateLoan(members[member_id].Loans[total_loans].amount, members[member_id].Loans[total_loans].period, members[member_id].Loans[total_loans].interestRate, loan);
	return loan;
} /* END of newLoan */
float calculateLoan(float amount, int period, float interestRate, float loan)
{	if (period == 0) return amount * loan;
	else
	{	loan *= (1+interestRate);
		calculateLoan(amount, period-1, interestRate, loan);
	}
} /* END of calculateLoan */
void getReport(int selection, int member_id)
{/*	FILE *ptr = fopen("customers.txt", "r");	
	if(selection == 1)
	{	int counter = 0, saved_id;
		while (!'\0')
		{	if(fscanf(ptr, "%d", &saved_id) == member_id)
			{	printf("%s", file);
		}
	}
	else
	{
	}*/
} /* END of getReport */
