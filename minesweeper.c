#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
#define SIZE_MAX 10

// background color
#define KNRM  "\x1B[0m"
#define BRED  "\x1B[41m"
#define BGRN  "\x1B[42m"
#define BYEL  "\x1B[43m"
#define BBLU  "\x1B[44m"
#define BMAG  "\x1B[45m"
#define BCYN  "\x1B[46m"
#define BWHT  "\x1B[47m"
// text color
#define KRED  "\x1B[31m"
#define KGREEN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


// global variables
// game table
unsigned char TableArray[SIZE_MAX][SIZE_MAX];
// location of cursor
int x=0, y=0;
// flag: input mode = 0, flag mode = 1, check mode = 2
int GameMode=0;

/*This is a recursive function which uncovers blank cells while they are adjacent*/
int UncoverBlankCell(int row, int col)
{
    int value, rows[8], columns[8], i;

    if(TableArray[row][col] != 0)
        return 0; // error

    TableArray[row][col] += 10; // uncover current cell

    // Get position of adjacent cells of current cell
    rows[0] = row - 1;
    columns[0] = col + 1;
    rows[1] = row;
    columns[1] = col + 1;
    rows[2] = row + 1;
    columns[2] = col + 1;
    rows[3] = row - 1;
    columns[3] = col;
    rows[4] = row + 1;
    columns[4] = col;
    rows[5] = row - 1;
    columns[5] = col - 1;
    rows[6] = row;
    columns[6] = col - 1;
    rows[7] = row + 1;
    columns[7] = col - 1;

    for(i = 0; i < 8; i++) {
        value = TableArray[rows[i]][columns[i]];

        if( (rows[i] >= 0 && rows[i] < SIZE_MAX) && (columns[i] >= 0 && columns[i] < SIZE_MAX) ) {		// to prevent negative index and out of bounds
            if(value > 0 && value <= 8)
                TableArray[rows[i]][columns[i]] += 10;										// it is a cell with 1-8 number so we need to uncover
            else if(value == 0)
                UncoverBlankCell(rows[i], columns[i]);
        }

    }

    return 1; // success!
}

void print_table()
{
    // clear screen
    system("clear");

    int i, j, value;
    for(i = 0; i < SIZE_MAX; i++) {
        for(j = 0; j < SIZE_MAX; j++) {
            if(x == j && y == i) {
                if(GameMode == 1) {
                    printf("|%sF%s",BMAG,KNRM);
                    continue;
                } else if(GameMode == 2) {
                    printf("|%sC%s",BMAG,KNRM);
                    continue;
                }

            }
            value = TableArray[i][j];

            if((value >= 0 && value <= 8) || value == 0 || value == 99)
                printf("|X");
            else if(value == 10) // clean area
                printf("|%s%d%s",KCYN, value - 10,KNRM);
            else if(value == 11) // the number of near mine is 1
                printf("|%s%d%s",KYEL, value - 10,KNRM);
            else if(value > 11 && value <= 18) // the number of near mine is greater than 1
                printf("|%s%d%s",KRED, value - 10,KNRM);
            else if((value >= 20 && value <= 28) || value == 100)
                printf("|%sF%s",KGREEN,KNRM);
            else
                printf("ERROR"); // test purposes

        }
        printf("|\n");
    }

    printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n",KCYN,KNRM,KGREEN,KNRM);
    if(GameMode == 0) {
        printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
    } else if(GameMode == 1) {
        printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
    } else if(GameMode == 2) {
        printf("Enter (select to check cell), q (Exit selection): ");
    }


}


int main(int argc, char *argv[])
{

    char ch;
    int nMines; // the number of the remaining mines
    int i,j,r,c,value, rows[8], columns[8];

NewGame:
    // the number of mines
    nMines = 10;
    if(argc == 2) {
        nMines = atoi(argv[1]);
    }
    srand (time(NULL));						// random seed
    // setting cursor
    x = 0;
    y = 0;
    // set all cells to 0
    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
            TableArray[i][j] = 0;

    for(i = 0; i < nMines; i++) {
        /* initialize random seed: */

        r = rand() % 10;					// it generates a integer in the range 0 to 9
        c = rand() % 10;

        // put mines
        if(TableArray[r][c] != 99) {
            TableArray[r][c] = 99;

            // Get position of adjacent cells of current cell
            rows[0] = r - 1;
            columns[0] = c + 1;
            rows[1] = r;
            columns[1] = c + 1;
            rows[2] = r + 1;
            columns[2] = c + 1;
            rows[3] = r - 1;
            columns[3] = c;
            rows[4] = r + 1;
            columns[4] = c;
            rows[5] = r - 1;
            columns[5] = c - 1;
            rows[6] = r;
            columns[6] = c - 1;
            rows[7] = r + 1;
            columns[7] = c - 1;

            for(j = 0; j < 8; j++) {
                value = TableArray[rows[j]][columns[j]];
                if( (rows[j] >= 0 && rows[j] < SIZE_MAX) && (columns[j] >= 0 && columns[j] < SIZE_MAX) ) {		// to prevent negative index and out of bounds
                    if(value != 99)																// to prevent remove mines
                        TableArray[rows[j]][columns[j]] += 1;									// sums 1 to each adjacent cell
                }
            }

        } else {								// to make sure that there are the properly number of mines in table
            i--;
            continue;
        }
    }

    //
    while(nMines != 0) {				// when nMines becomes 0 you will win the game
        print_table();

        ch = getch();
        // cursor direction
        char direction;
        switch (ch) {

        // flag
        case 'f':
        case 'F':


FlagMode:
            GameMode = 1;
            do {
                print_table();
                direction = getch();
                // arrow direction
                if(direction == '8') {
                    // up
                    y = (SIZE_MAX + --y) % SIZE_MAX;
                } else if(direction == '2') {
                    // down
                    y = ++y % SIZE_MAX;
                } else if(direction == '4') {
                    x = (SIZE_MAX + --x) % SIZE_MAX;
                } else if(direction == '6') {
                    x = ++x % SIZE_MAX;
                } else if(direction == 'c' || direction == 'C') {
                    goto CheckMode;
                } else if(direction == '\n') {
                    value = TableArray[y][x];

                    if (value == 99) {					// mine case
                        TableArray[y][x] += 1;
                        nMines -= 1;				// mine found
                    } else if(value >= 0 && value <= 8) {	// number of mines case (the next cell is a mine)
                        TableArray[y][x] += 20;
                    } else if(value >= 20 && value <= 28) {
                        TableArray[y][x] -= 20;
                    }

                    if(nMines == 0)
                        break;
                }
            } while (direction != 'q' && direction != 'Q');
            GameMode = 0;

            break;

        // check cell
        case 'c':
        case 'C':

CheckMode:
            GameMode = 2;
            do {
                print_table();
                direction = getch();

                // arrow direction
                if(direction == '8') {
                    // up
                    y = (SIZE_MAX + --y) % SIZE_MAX;
                } else if(direction == '2') {
                    // down
                    y = ++y % SIZE_MAX;
                } else if(direction == '4') {
                    x = (SIZE_MAX + --x) % SIZE_MAX;
                } else if(direction == '6') {
                    x = ++x % SIZE_MAX;
                } else if(direction == 'f' || direction == 'F') {
                    goto FlagMode;
                }

                else if(direction == '\n') {
                    value = TableArray[y][x];
                    if(value == 0)						// blank case
                        UncoverBlankCell(y, x);
                    else if(value == 99)				// mine case
                        goto EndOfGame;
                    else if(value > 0 && value <= 8)	// number case (the next cell is a mine)
                        TableArray[y][x] += 10;

                    //	break;
                }
            } while (direction != 'q' && direction != 'Q');
            GameMode = 0;

            break;



        // jump to a new game
        case 'n':
        case 'N':
            goto NewGame;
            break;

        // exit
        case 'q':
        case 'Q':
            goto EndOfGame;

        default:
            break;
        }
    }

EndOfGame:
    GameMode = 0;
    print_table();
    printf("\nGAME OVER\n");

    if(nMines == 0)
        printf("you won!!!!\n");

    else
        printf("BOOM! you LOOSE!\n");

    do {
        printf("Are you sure to exit? (y or n)? ");
        ch = getch();
        putchar('\n');
        if(ch == 'y' || ch == 'Y') {
            break;
        } else if(ch == 'n' || ch == 'N') {
            goto NewGame;
        }
        printf("Please answer y or n\n");
    } while(1);
    printf("See you next time!\n");

    fflush(stdin);

    return 0;
}
