/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Exit with error code if incorrect number of arguments are supplied.  Need either 2 (no seed specified) or 3 (with seed specified).
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // convert number of random integers required (arg 1) to an int, and assign it to n
    int n = atoi(argv[1]);

    // if user has specified a seed, use it to seed srand48 (required to initialise drand48).  If user has not, then use the time function as a pseudo-random seed.
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // For each random integer required (n), generate a random float using drand48 (which will be between 0.0 and 1.0) and multiply by upper limit to give us our random int.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
