/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX]; //must be board[row][column] for check50 to work.  Space must be represented by 0.

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(50000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(200000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int num = (d * d) - 1;

    //for each row (i)
    for (int i = 0; i < d; i++)
    {
        //for each column (j)
        for (int j = 0; j < d; j++)
        {
            //assign a number, then decrease number by 1
            board[i][j] = num;
            num--;
        }
    }


    if (d % 2 == 0)
    {
        //then odd number of spaces and we have to swap the 1 and the 2 in the board array - otherwise the game is impossible
        //(this is an odd number of spaces because you subtract 1 from d x d.  So if d is 4, 4x4 = 16 less 1 is 15 = odd.)
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //for each row (i)
    for (int i = 0; i < d; i++)
    {
        //for each column (j)
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] != 0)
            {
                //if the number is single digit we have to add an extra space for alignment
                if (board[i][j] < 10)
                {
                    printf(" ");
                }

                //print the number
                printf("%i  ", board[i][j]);
            }
            else
            {
                //don't print the zero character - it's the blank space
                printf("    ");
            }
        }

        //next line
        printf("\n");
    }

    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{

    // open log
    FILE *fmove = fopen("lmove.txt", "w");
    if (fmove == NULL)
    {
        return 3;
    }
    fprintf(fmove, "START MOVE FUNCTION, TEST TILE IS %i.\n", tile);
    fflush(fmove);


    //return false if tile is out of range
    if (tile > (d * d) - 1 || tile < 1)
    {
        fprintf(fmove, "Tile is out of range, exiting.\n");
        fflush(fmove);
        return false;
    }

    //search the board for the selected tile
    //for each row (i)
    for (int i = 0; i < d; i++)
    {
        //for each column (j)
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                fprintf(fmove, "Found tile in pos %i, %i.\n", i, j);
                fflush(fmove);
                //check surrounding tiles.

                //UP
                if (i > 0)
                {
                    if (board[i-1][j] == 0)
                    {
                        board[i][j] = 0;
                        board[i-1][j] = tile;
                        return true;
                    }
                }

                //RIGHT
                if (j < d-1)
                {
                    if (board[i][j+1] == 0)
                    {
                        board[i][j] = 0;
                        board[i][j+1] = tile;
                        return true;
                    }
                }

                //DOWN
                if (i < d-1)
                {
                    if (board[i+1][j] == 0)
                    {
                        board[i][j] = 0;
                        board[i+1][j] = tile;
                        return true;
                    }
                }

                //LEFT
                if (j > 0)
                {
                    if (board[i][j-1] == 0)
                    {
                        board[i][j] = 0;
                        board[i][j-1] = tile;
                        return true;
                    }
                }

            }
        }
    }

    fclose(fmove);

    return false;

}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{

    int k = 1;

    //for each row (i)
    for (int i = 0; i < d; i++)
    {
        //for each column (j)
        for (int j = 0; j < d; j++)
        {
            if (i == d-1 && j == d-1)
            {
                //we are at the bottom-right tile so we have won.  Don't test the last because it won't increment by 1 like all the others (it will be zero)
                return true;
            }
            else if (board[i][j] != k)
            {
                return false;
            }

            k++;
        }
    }

    return true;
}
