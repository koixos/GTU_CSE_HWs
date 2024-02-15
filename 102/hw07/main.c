#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 26
#define WORD_MAX 6
#define NUM_OF_WORDS 7

enum Direction{upward = 1, downward, left, right, u_l_diagonal, d_l_diagonal, u_r_diagonal, d_r_diagonal};
void choose_random_words(char words[][WORD_MAX]);						/* A function for choosing random words.						*/
void start_coordinates(int *coordinates);						/* A function for determining first index of a word.				*/
int check_if_valid(enum Direction direction, int *current_position,			/* A function for checking if a word fits.					*/
		int *coordinates, int size);				
void fit_words(enum Direction direction, int table[][15],					/* A function for fitting words by calling 'start_coordinates' and 'check_if_valid'. */
		int *coordinates, int size);
void print_board(int table[][15]);							/* A function for printing the board.						*/
void board_build(char words[][WORD_MAX], int table[][15], int *coordinates,		/* A function for building the table.						*/
							int all_coordinates[][2]);		
void delete_word(enum Direction direction, int table[][15], int size, int row, int column); /* A function for deleting found words.						*/
void play_game(int table[][15]);								/* A function for playing the game.						*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main()
{	srand(time(NULL));
	char words[NUM_OF_WORDS][WORD_MAX];						/* This char-array will hold random words which are taken from the list.		*/
	int exit = 0, table[15][15], coordinates[2], all_coordinates[NUM_OF_WORDS][2];	/* 'coordinates[2]' will hold first coordinates of words				*/
	choose_random_words(words);							/* NOTE: coordinate[0] will always represent x-coordinate or "row"		*/
	board_build(words, table, coordinates, all_coordinates);				/* 	coordinate[1] will always represent y-coordinate or "column"		*/
	printf("\nHidden words:\n");
	for (int i = 0; i < NUM_OF_WORDS; i++) printf("%s --> [%d, %d]\n", words[i], all_coordinates[i][0], all_coordinates[i][1]);				
	play_game(table);								
	return 0;
} /* END of main */
void choose_random_words(char words[][WORD_MAX])				
{
	FILE *ptr;
	int i, numbers[NUM_OF_WORDS], temp[NUM_OF_WORDS];					/* This array will hold numbers which are pointers to lines of the wordlist.	*/
	for (i = 0; i < NUM_OF_WORDS; i++)						/* This loop will create 7 random numbers range of 1-50.				*/
	{	numbers[i] = 1 + rand() % 50;						/* These numbers will reference which lines of the list will be taken for the game. */
		temp[i] = numbers[i];							/* Array "numbers" is copied to array "temp" for comparison.			*/
	}
	for (i = 0; i < NUM_OF_WORDS; i++)
	{	for (int j = i+1; j < NUM_OF_WORDS; j++)					/* Inner loop should start with 'i+1'.						*/
		{	if(numbers[i] == temp[j])					/* We'll be able to compare each number in 'number' through this algorithm.	*/
			{	numbers[i] = 1 + rand() % 50;				/* It simply compares all the indexes of 'numbers' and 'temp' to each other,	*/
				temp[i] = numbers[i];					/* ... and if they are same, it changes the value of numbers[i] and finally counter */
				i = 0;							/* ... is resetted to start again to be able to compare the changed value to others.*/
			}
		}
	}
	for (i = 0; i < NUM_OF_WORDS; i++)
	{	int counter = 1;							/* The line-counter should be reseted for each loop.				*/
		ptr = fopen("wordlist.txt", "r");
		do									/* In this do-while, we're saying that read every line until line-counter is equal to */
		{	fscanf(ptr, "%s", words[i]);					/* ... the ith index of array "numbers".						*/
			++counter;							/* And while reading, make ith index of array "words" that line.			*/
		}while (counter <= numbers[i]);						/* And so, we'll have 7 random words taken from the list for each game.		*/
		fclose(ptr);
	}
} /* END of choose_random_words */
void start_coordinates(int *coordinates)
{	for (int i = 0; i < 2; i++) coordinates[i] = 0 + rand() % 15;			/* This loop will create 2 coordinate numbers range of 0-14.			*/
} /* END of start_coordinates
  * "check_if_valid" function returns two different values:
  * 	# -999 meaning either wanted coordinate of table is not empty or out of borders.
  * 	# <direction> meaning wanted coordinate is available.														*/
int check_if_valid(enum Direction direction, int *current_position, int *coordinates, int size)
{											/* NOTE: The table will be filled with letters of words in "board_build" fucntion	*/
	switch(direction)								/* 	... according to these instructions below.				*/
	{
		case upward:								/* up to down --> column fixed, row will increase					*/
			if(*current_position != ' ' || coordinates[0] + size > 14) 	
				return -999;
			else return upward;
		case downward:								/* down to up --> column fixed, row will decrease					*/
			if(*current_position != ' ' || coordinates[0] - size < 0) 	
				return -999;
			else return downward;
		case left: 								/* left to right --> row fixed, column will increase				*/
			if(*current_position != ' ' || coordinates[1] + size > 14)
				return -999;
			else return left;
		case right:								/* right to left --> row fixed, column will decrease				*/
			if(*current_position != ' ' || coordinates[1] - size < 0) 	
				return -999;
			else return right;
		case u_l_diagonal:							/* up-left to down-right --> both row and column will increase			*/
			if(*current_position != ' ' || coordinates[0] + size > 14
						   || coordinates[1] + size > 14)
				return -999;
			else return u_l_diagonal;
		case d_l_diagonal:							/* down-left to up-right --> row will decrease, column will increase		*/
			if(*current_position != ' ' || coordinates[0] - size < 0
						   || coordinates[1] + size > 14)
				return -999;
			else return d_l_diagonal;		
		case u_r_diagonal:							/* up-right to down-left --> row will increase, column will decrease		*/
			if(*current_position != ' ' || coordinates[0] + size > 14
						   || coordinates[1] - size < 0)
				return -999;		
			else return u_r_diagonal;
		case d_r_diagonal:							/* down-right to up-left --> both row and column will decrease			*/
			if(*current_position != ' ' || coordinates[0] - size < 0
						   || coordinates[1] - size < 0)
				return -999;
			else return d_r_diagonal;
	}
} /* END of check_if_valid 
  * "fit_words" function will check whether a determined coordinate is available or not by calling "start_coordinates" and "check_if_valid" functions.
  * First, "fit_words" will call "start_coordinate" to determine a start-coordinate.
  * Then, it'll send that coordinate to "check_if_valid" to check if the coordinate can be used.
  * We know that if a coordinate is not available "check_if_valid" will return -999.
  * So; if "check_if_valid" returns -999, "fit_words" will call "start_coordinates" again to determine a new coordinate.
  * Since whole function is in a while-loop, this will continue until a valid coordinate is determined.
  * And when it determined, while-loop will be quitted, and so "fit_words" will be done;
  * ... and we'll be able to use determined valid coordinate in "board_build" function since we declared it there as an array.						*/
void fit_words(enum Direction direction, int table[][15], int *coordinates, int size)
{	int exit = 0, returned;
	while (exit == 0)
	{
		start_coordinates(coordinates);
		returned = check_if_valid(direction, &table[coordinates[0]][coordinates[1]], coordinates, size);
		if(returned != -999) exit = 1;
	}
} /* END of fit_words */
void print_board(int table[][15])
{
	for (int row = 0; row < 15; row++)							
	{	for (int column = 0; column < 15; column++)
		{	if(table[row][column] == 'X') printf("\e[0;31m%c\e[0m ", table[row][column]);		
			else printf("%c ", table[row][column]);	
		}		
		printf("\n");
	}
} /* END of print_board */
void board_build(char words[][WORD_MAX], int table[][15], int *coordinates, int all_coordinates[][2])							
{
	enum Direction direction;
	int i, j, row, column;
	for (row = 0; row < 15; row++)							
	{	for (column = 0; column < 15; column++)					/* First we'll fill the whole table with space - kinda cleaning it.		*/
			table[row][column] = ' ';					/* This will help us with coordinate validation. 					*/
	}	
	for (i = 0; i < NUM_OF_WORDS; i++)
	{
		int size = strlen(words[i]), exit = 0, flag = 0;
		direction = 1 + rand() % 8;						/* We'll determine a random direction for each word, 				*/
		fit_words(direction, table, coordinates, size);				/* ... and then send it to "fit_words" to determine a valid coordinate there.	*/
		while (exit == 0)							/* And we'll make some operations depending on the direction and the coordinate.	*/
		{	if(direction == upward)						
			{	for (j = 0; j < size; j++)				/* j is used as letter index --> words[i][j]					*/
				{	if(table[coordinates[0] + j][coordinates[1]] != ' ') /* Here we're looking for if coordinates which the word will be locating is empty. */
						flag = 1;				/* And even if one of them is not available, the flag will turn to "1".		*/
				}
				if(flag != 0)						/* If flag isnot "0" meaning at least one letter-coordinate isnot empty,		*/
				{	fit_words(direction, table, coordinates, size);	/* ... we'll call "fit_words" again to determine a new start-coordinate.		*/
					flag = 0;					/* resetting 'flag' for other words						*/
				}
				else							/* If flag is "0" meaning all letter-coordinates are empty and can be used,	*/
				{	for (j = 0; j < size; j++)			/* ... we'll fill the table's those coordinates with word's letters.		*/
						table[coordinates[0] + j][coordinates[1]] = words[i][j];
					exit = 1;					
				}
			}								/* NOTE: These fillings'll be done according to the instructions in "check_if_valid" */
			else if(direction == downward)					/* And we'll repeat these operations again and again for each word.		*/
			{	for (j = 0; j < size; j++)
				{	if(table[coordinates[0] - j][coordinates[1]] != ' ')
						flag = 1;
				}
				if(flag != 0)
				{	fit_words(direction, table, coordinates, size);
					flag = 0;
				}
				else
				{	for (j = 0; j < size; j++)
						table[coordinates[0] - j][coordinates[1]] = words[i][j];
					exit = 1;
				}
			}
			else if(direction == left)	
			{	for (j = 0; j < size; j++)
				{	if(table[coordinates[0]][coordinates[1] + j] != ' ')
						flag = 1;
				}
				if(flag != 0)
				{	fit_words(direction, table, coordinates, size);
					flag = 0;
				}
				else
				{	for (j = 0; j < size; j++)
						table[coordinates[0]][coordinates[1] + j] = words[i][j];
					exit = 1;
				}	
			}
			else if(direction == right)
			{	for (j = 0; j < size; j++)
				{	if(table[coordinates[0]][coordinates[1] - j] != ' ')
						flag = 1;
				}
				if(flag != 0)
				{	fit_words(direction, table, coordinates, size);
					flag = 0;
				}
				else
				{	for (j = 0; j < size; j++)
						table[coordinates[0]][coordinates[1] - j] = words[i][j];
					exit = 1;
				}					
			}
			else if(direction == u_l_diagonal)
			{	for (j = 0; j < size; j++)
				{	if(table[coordinates[0] + j][coordinates[1] + j] != ' ')
						flag = 1;
				}
				if(flag != 0)
				{	fit_words(direction, table, coordinates, size);
					flag = 0;
				}
				else
				{	for (j = 0; j < size; j++)
						table[coordinates[0] + j][coordinates[1] + j] = words[i][j];
					exit = 1;
				}
			}
			else if(direction == d_l_diagonal)
			{	for (j = 0; j < size; j++)
				{	if(table[coordinates[0] - j][coordinates[1] + j] != ' ')
						flag = 1;
				}
				if(flag != 0)
				{	fit_words(direction, table, coordinates, size);
					flag = 0;
				}
				else
				{	for (j = 0; j < size; j++)
						table[coordinates[0] - j][coordinates[1] + j] = words[i][j];
					exit = 1;
				}
			}
			else if(direction == u_r_diagonal)
			{	for (j = 0; j < size; j++)
				{	if(table[coordinates[0] + j][coordinates[1] - j] != ' ')
						flag = 1;
				}
				if(flag != 0)
				{	fit_words(direction, table, coordinates, size);
					flag = 0;
				}
				else
				{	for (j = 0; j < size; j++)
						table[coordinates[0] + j][coordinates[1] - j] = words[i][j];
					exit = 1;
				}
			}
			else if(direction == d_r_diagonal)
			{	for (j = 0; j < size; j++)
				{	if(table[coordinates[0] - j][coordinates[1] - j] != ' ')
						flag = 1;
				}
				if(flag != 0)
				{	fit_words(direction, table, coordinates, size);
					flag = 0;
				}
				else
				{	for (j = 0; j < size; j++)
						table[coordinates[0] - j][coordinates[1] - j] = words[i][j];
					exit = 1;
				}
			}
		} all_coordinates[i][0] = coordinates[0]; all_coordinates[i][1] = coordinates[1];
	} 
	for (row = 0; row < 15; row++)							
		for (column = 0; column < 15; column++)
			if(table[row][column] == ' ')					/* We'll fill the gaps with random characters.					*/
				table[row][column] = 97 + rand() % 26;
	print_board(table);								/* "print_board" will be called here once to start the game.			*/
} /* END of board_build */
void delete_word(enum Direction direction, int table[][15], int size, int row, int column)	/* After founding a word successfully, "delete_word" will take its direction	*/
{	for (int i = 0; i < size; i++)							/* ...  and size, and just replace the table's related coordinates with a red "X".	*/
	{	switch(direction)
		{	case upward:							
				table[row+i][column] = 'X'; break;			/* NOTE: All replacements will be done according to the instructions in		*/
			case downward:							/* 	... "check_if_valid".							*/
				table[row-i][column] = 'X'; break;
			case left:
				table[row][column+i] = 'X'; break;
			case right:
				table[row][column-i] = 'X'; break;
			case u_l_diagonal:
				table[row+i][column+i] = 'X'; break;
			case d_l_diagonal:
				table[row-i][column+i] = 'X'; break;
			case u_r_diagonal:
				table[row+i][column-i] = 'X'; break;
			case d_r_diagonal:
				table[row-i][column-i] = 'X'; break;	
		}
	}
} /* END of delete_word */
void play_game(int table[][15])
{
	int i, row, column, exit = 0, mistakes = 0, point = 0;
	char word_found[WORD_MAX];
	while (exit == 0)
	{	int u_c = 0, d_c = 0, l_c = 0, r_c = 0, u_l_c = 0, d_l_c = 0, u_r_c = 0, d_r_c = 0;	/* We'll use different letter-counters for each direction.	*/
		printf("------------------------------------------------------------------\n");
		printf("Enter first coordinates of word and word itself (type\":q\" to exit)\n");
		printf("------------------------------------------------------------------\nRow: "); scanf("%d", &row);
		printf("Column: "); scanf("%d", &column);
		printf("Word: "); scanf("%s", word_found);
		if(row == ':' || column == ':' || word_found[0] == ':') { printf("Total point: %d\n", point); exit = 1; }
		else
		{	int size = strlen(word_found);
			for (i = 0; i < size; i++)							/* Here we are looking for the word for each direction.		*/
			{	if(table[row+i][column] == word_found[i]) ++u_c;				/* If the letter on a coordinate is equal to ith letter of word,	*/
				if(table[row-i][column] == word_found[i]) ++d_c;				/* ... we'll increase counters of those directions by one.	*/
				if(table[row][column+i] == word_found[i]) ++l_c;				/* And so, all the directions will be searched for the word.	*/
				if(table[row][column-i] == word_found[i]) ++r_c;					
				if(table[row+i][column+i] == word_found[i]) ++u_l_c;
				if(table[row-i][column+i] == word_found[i]) ++d_l_c;
				if(table[row+i][column-i] == word_found[i]) ++u_r_c;
				if(table[row-i][column-i] == word_found[i]) ++d_r_c;
			}
			if(u_c == size || d_c == size || l_c == size || r_c == size
			   || u_l_c == size || d_l_c == size || u_r_c == size || d_r_c == size)		/* After searching each direction in itself, if the start-coordinate */
			{	point += 2;								/* ... player gave us is correct and the word is somewhere in there, */
				printf("Founded! You got 2 points.\nTotal point: %d\n", point);		/* ... then one of counters should be equal to the size of the word. */
				if(u_c == size) delete_word(upward, table, size, row, column);		/* We'll send that direction to "delete_word" to delete the word.	*/
				else if(d_c == size) delete_word(downward, table, size, row, column);	/* And also total points of players will be increase by 2.	*/
				else if(l_c == size) delete_word(left, table, size, row, column);
				else if(r_c == size) delete_word(right, table, size, row, column);	/* If either the word or start-coordinate player gave us is incorrect*/
				else if(u_l_c == size) delete_word(u_l_diagonal, table, size, row, column);/* ... then no counter can be equal to the size of the word	*/
				else if(d_l_c == size) delete_word(d_l_diagonal, table, size, row, column);/* ... since there is no word.					*/
				else if(u_r_c == size) delete_word(u_r_diagonal, table, size, row, column);/* So we'll print an error message which includes how many mistakes */
				else if(d_r_c == size) delete_word(d_r_diagonal, table, size, row, column);/* ... player can still make.					*/
			}										/* And if total mistakes become 3, then the game will be done and	*/
			else										/* ... player will lose.					*/
			{	++mistakes;								/* If player can find all the words, total point will get to 14	*/
				if(mistakes == 3){ printf("You lost!\n"); exit = 1; }			/* ... and so the game will be done and player will win.		*/
				else printf("Wrong choice! You have only %d left.\n", 3-mistakes);		
			}											
			if(point == 14){ printf("You won!\n"); exit = 1; }
			print_board(table);
		}
	}
} /* END of play_game */
