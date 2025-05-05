#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define RESET "\e[0m"

void createMap(char arr[][30], int x1, int y1, int x2, int y2)
{
	for (int row = 0; row < 15; row++)
	{
		for (int column = 0; column < 30; column++)
		{
			if(row == 0 || row == 14) arr[row][column] = '-';						/* outer border - horizontal */
			else if(column == 0 || column == 29) arr[row][column] = '|'; 					/* outer border - vertical */
			else if((2 <= column && column <= 27) && (row == 2 || row == 12)) arr[row][column] = '-';	/* first inner border - horizontal */
			else if((2 <= row && row <= 12) && (column == 2 || column == 27)) arr[row][column] = '|';	/* first inner border - vertical   */
			else if((4 <= column && column <= 25) && (row == 4 || row == 10)) arr[row][column] = '-'; 	/* second inner border - horizontal*/
			else if((4 <= row && row <= 10) && (column == 4 || column == 25)) arr[row][column] = '|';	/* second inner border - vertical  */
			else if((row==1&&column==14)||(row==7&&column==28)||(row==13&&column==14)) arr[row][column] = 'X';/* penalty points for PLAYER 1 */
			else if((row==3&&column==10)||(row==3&&column==18)||(row==5&&column==26)||
				(row==10&&column==26)||(row==11&&column==10)||(row==11&&column==18)) arr[row][column] = 'X';/* penalty points for PLAYER 2 */
			else if((row==2&&column==1)||(row==4&&column==3)) arr[row][column] = '='; 			/* finish lines */
			else if(row == x1 && column == y1) arr[row][column] = '1'; 					/* position of PLAYER 1 */
			else if(row == x2 && column == y2) arr[row][column] = '2'; 					/* position of PLAYER 2 */
			else arr[row][column] = ' ';	
		}
	}
} /* END of createMap */
void printMap(char arr[][30], int x1, int y1, int x2, int y2)
{	
	createMap(arr, x1, y1, x2, y2); /* We'll call createMap for each dice to create a new map everytime */
	for (int row = 0; row < 15; row++)
	{	for (int column = 0; column < 30; column++)
		{
			if(arr[row][column] == '1') printf(BLU "%c" RESET, arr[row][column]);
			else if(arr[row][column] == '2') printf(YEL "%c" RESET, arr[row][column]);
			else if(arr[row][column] == 'X') printf(RED "%c" RESET, arr[row][column]);
			else if(arr[row][column] == '=') printf(GRN "%c" RESET, arr[row][column]);
			else printf("%c", arr[row][column]);
		}
		printf("\n");
	}
} /* END of printMap */
int dice()
{	
	int dice = 1;		/* min value of the dice */
	dice += rand() % 6;	/* rand() will create numbers range of 0 to 5 and for the final result it'll be added to the dice's minimum value */
	return dice;
} /* END of dice */
int dice_to_play(int player)
{
	int returned;
	if(player == 1) printf(BLU);
	else if(player == 2) printf(YEL);
	printf("Player %d - press ENTER to dice", player);
	getchar();
	returned = dice();
	printf("DICE: %d\n\n", returned);
	printf(RESET);
	return returned;
} /* END of dice_to_play */
int startGame(int p1, int p2)
{
	if(p1 == p2) return 0;
	else if(p1 > p2) return 1;
	else if(p2 > p1) return 2;
} /* END of startGame */
int main()
{
	srand(time(NULL));
	
	char arr[15][30];
	/* These flags will allow us to use IFs/ELSEs as while loops */
	int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0, flag6 = 0, flag7 = 0, /* Flags for PLAYER 1 */
	    Flag1 = 0, Flag2 = 0, Flag3 = 0, Flag4 = 0, Flag5 = 0, Flag6 = 0, Flag7 = 0; /* Flags for PLAYER 2 */
	int exit = 0, vertical1 = 1, horizontal1 = 1, vertical2 = 3, horizontal2 = 3, dice_p1, dice_p2, returned, subtraction;
	
	printMap(arr, vertical1, horizontal1, vertical2, horizontal2); /* calling printMap with players' initial position */
	printf("\n\nTo start to game, players should dice and decide who is going to start first.\n\n");
	while (exit == 0)
	{
		dice_p1 = dice_to_play(1);
		dice_p2 = dice_to_play(2);
		returned = startGame(dice_p1, dice_p2); /* sending dice values to dice() to decide who goes first */
		/* If we get a valid value (1 or 2), while will be broken; otherwise (means 0) we'll continue to dice */
		if(returned == 0) printf("\nSame dice value -- Try again\n\n");
		else if(returned == 1 || returned == 2)
		{
			printf("\n ### PLAYER %d will start the game ###\n\n", returned);
			exit = 1;
		}
	}
	while (exit == 1)
	{
		if(returned == 1)
		{
			dice_p1 = dice_to_play(1); 
			dice_p2 = dice_to_play(2);
		}
		else if(returned == 2)
		{
			dice_p2 = dice_to_play(2); 
			dice_p1 = dice_to_play(1);
		}
		/* for PLAYER 1 */
		if(flag1 == 0 && (horizontal1 += dice_p1) <= 28) 				/* if it is still in borders */
		{
			if(horizontal1 == 14)							/* penalty point */
			{
				horizontal1 -= 2;
				printf("Penalty for Player 1\n");
			}				
		}
		else
		{	flag1 = 1; /* We'll set flag1 on "1" to make sure upper if will not be valid again, like quitting from a while with an exit code */
			if(flag2 == 0) 								/* this will allow us to make this operation for only one time */
			{
				subtraction = horizontal1-28; 					/* we'll find how much it is passing the border */
				horizontal1 = 28; 						/* and then we'll fasten horizontal1 to 28th row */
				vertical1 += subtraction; 					/* we'll add that excess to vertical1 to change its coordinate */
				if(vertical1 == 7)						/* penalty point */
				{
					vertical1 -= 2;
					printf("Penalty for Player 1\n");
				}				
				flag2 = 1;
			}
			else
			{	if(flag3 == 0 && (vertical1 += dice_p1) <= 13) 			/* if it is still in borders */
				{
					if(vertical1 == 7)					/* penalty point */
					{
						vertical1 -= 2; 
						printf("Penalty for Player 1\n");
					}			
				}
				else
				{	flag3 = 1;
					if(flag4 == 0)
					{
						subtraction = vertical1-13;			/* we'll find how much it is passing the border, again */
						vertical1 = 13;					/* and then we'll fasten vertical1 to 13th column */
						horizontal1 -= subtraction;			/* we'll subtract that excess from horizontal1 */
						flag4 = 1;
					}
					else
					{	if(flag5 == 0 && (horizontal1 -= dice_p1) >= 1)
						{
							vertical1 = 13;
							if(horizontal1 == 14)			/* penalty point */
							{
								horizontal1 += 2;
								printf("Penalty for Player 1\n");
							}	
						}
						else
						{	flag5 = 1;
							if(flag6 == 0)
							{
								subtraction = 1-horizontal1;	/* We will repeat same thing again and again for every corner */
								horizontal1 = 1;
								vertical1 -= subtraction;
								flag6 = 1;
							}
							else
							{
								if(flag7 == 0 && (vertical1 -= dice_p1) > 2);
								else if(vertical1 <= 2)		/* finish line */
								{
									printf("\n ### PLAYER 1 WON THE GAME ###\n\n");
									exit = 0;
								}
							}
						}
					}
				}
			}
		}
		/* for PLAYER 2 */	
		if(Flag1 == 0 && (horizontal2 += dice_p2) <= 26) 				/* if it is still in borders */
		{
			if(horizontal2 == 10 || horizontal2 == 18)				/* penalty point */
			{
				horizontal2 -= 2; 	
				printf("Penalty for Player 2\n");
			}	
		} 
		else 									
		{	Flag1 = 1;
			if(Flag2 == 0) 								/* this will allow us to make this operation for only one time */
			{
				subtraction = horizontal2-26; 					/* we'll find how much it is passing the border */
				horizontal2 = 26; 						/* and then we'll fasten horizontal2 to 26th row */
				vertical2 += subtraction;					/* we'll add that excess to vertical2 */
				if(vertical2 == 5) 						/* penalty point */
				{
					vertical2 -= 2; 
					printf("Penalty for Player 2\n");
				}				
				Flag2 = 1;
			}
			else
			{	if(Flag3 == 0 && (vertical2 += dice_p2) <= 11) 			/* if it is still in borders */
				{	
					if(vertical2 == 5 || vertical2 == 10) 			/* penalty point */
					{
						vertical2 -= 2; 
						printf("Penalty for Player 2\n");
					}	
				}
				else
				{	Flag3 = 1;
					if(Flag4 == 0)
					{
						subtraction = vertical2-11;			/* we'll find how much it is passing the border */
						vertical2 = 11;					/* and then we'll fasten vertical2 to 11th column */
						horizontal2 -= subtraction;			/* we'll subtract that excess from horizontal2 */
						Flag4 = 1;
					}
					else
					{	if(Flag5 == 0 && (horizontal2 -= dice_p2) >= 3)	/* if it is still in borders */
						{
							vertical2 = 11;
							if(horizontal2 == 10 || horizontal2 == 18)/* penalty point */	 
							{
								horizontal2 += 2; 
								printf("Penalty for Player 2\n");
							}
						}
						else
						{	Flag5 = 1;
							if(Flag6 == 0)
							{
								subtraction = 3-horizontal2;	/* We will repeat same thing again and again for every corner */
								horizontal2 = 3;
								vertical2 -= subtraction;
								Flag6 = 1;
							}
							else
							{	if(Flag7 == 0 && (vertical2 -= dice_p2) > 4);	
								else if(vertical2 <= 4)		/* finish line */
								{
									printf("\n ### PLAYER 2 WON THE GAME ###\n\n");
									exit = 0;
								}
							}
						}
					}
				}
			}
		}
		printMap(arr, vertical1, horizontal1, vertical2, horizontal2);	
	}
	return 0;
}
