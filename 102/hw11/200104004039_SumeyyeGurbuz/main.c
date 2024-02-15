#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct table {
	char **field;
	char **type;
	bool *isNull;
	bool *isKey;
} table;

typedef struct tables {
	char *name;						/*	Table name					*/
	struct tables *next;
	table *t;
} tables;

typedef struct database {
	tables *tList;						/*	To be implemented as a linked list		*/
	int n;							/*	Num of entries					*/
	char name[20];						/*	Database name					*/
} database;

void create_database(char *name);				/*	Create a new database if not exist		*/

void show_table(database *d);					/*	Show tables on a database by names		*/

void desc_table(database *d, table *t);			/*	Describe tables				*/

void insert_table(database *d, table *t);			/*	Create a new table with columns on a database	*/

void remove_table(database *d, char *name);			/*	Remove a table from a database			*/

void insert_key(database *d, table *t, int key_value);	/*	Insert a key on a table			*/

int main()
{	database *db = (database*)malloc(sizeof(database));
	printf("Enter the name of the database: "); scanf("%s", db->name);
	printf("Enter the number of entries: "); scanf("%d", &db->n);
	
	return 0;
}

void create_database(char *name)
{	
}
