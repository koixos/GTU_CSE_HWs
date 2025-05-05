#include <stdio.h>

void takeInput(int *row, int *column, int *step) {
    printf("\nRow size: ");
    scanf("%d", row);
    printf("\nColumn size: ");
    scanf("%d", column);
    printf("\nStep number: ");
    scanf("%d", step);
    printf("\n");
}

void takeGrid(char *grid, int row, int column) {
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            scanf(" %c", &grid[i * column + j]);
    printf("\n");
}

void printGrid(char *grid, int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
            printf("%c ", grid[i * column + j]);
        printf("\n");
    }
    printf("\n");
}

void firstStep(char *grid, char *memory, int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (grid[i * column + j] == 'O')
                memory[i * column + j] = 'X';
            else 
                memory[i * column + j] = '.';
        }
    }    
}

void secondStep(char *grid, int row, int column) {
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            if (grid[i * column + j] == '.')
                grid[i * column + j] = 'O';
}

void thirdStep(char *grid, char *memory, int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (memory[i * column + j] == 'X') {
                grid[i * column + j] = '.';

                if (j != 0)     grid[i * column + j - 1] = '.';
                if (j != 15)    grid[i * column + j + 1] = '.';

                if (i != 0)     grid[(i - 1) * column + j] = '.';
                if (i != 15)    grid[(i + 1) * column + j] = '.';
            }
        }
    }
    firstStep((char*)grid, (char*)memory, row, column);
}

int main() {
    int row, column, step;
    takeInput(&row, &column, &step);

    char grid[row][column];
    takeGrid((char*)grid, row, column);

    int counter = 2;
    char mem[row][column];
    
    firstStep((char*)grid, (char*)mem, row, column);
    printGrid((char*)grid, row, column);

    while (counter <= step) {
        secondStep((char*)grid, row, column);
        ++counter;
        if (counter > step) {
            break;
        } else {
            thirdStep((char*)grid, (char*)mem, row, column);
            ++counter;
        }
    }

    printGrid((char*)grid, row, column);
}

/* 
 * Example Input:

....OO..OO..O.O.
.O.O...O........
................
.OOOO..........O
..O.O.O.O.......
..........O.....
.....O..O.......O
O.........O......
.......O.........
.................
.................
..........O....O.
O...........O....
..........O.OO...
OOOOOOOOOOOOOOOOO
................O

*/