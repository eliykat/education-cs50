/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{

    // BINARY SEARCH ALGORITHM

    int start = 0;
    int end = n - 1; //n is the number of elements, not the index
    int mid;

    while (start <= end)
    {
        mid = start + (end-start)/2;

        if (values[mid] == value)
        {
            return true;
        }
        else if (values[mid] > value)
        {
            end = mid - 1;
        }
        else if (values[mid] < value)
        {
            start = mid + 1;
        }

    }

    return false;

    /* SIMPLE LINEAR SEARCH
    int i;

    for (i = 0; i < n; i++)
    {
        if (values[i] == value)
        {
            return true;
        }
    }

    return false;
    */
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{

    //COUNT SORT ALGORITHM
    const long MAX = 65537; //equal to the highest possible value plus 1 (to correct for the fact that an array will start at 0)

    //initialise count array
    int count[MAX];

    for (int i = 0; i < MAX; i++)
    {
        count[i] = 0;
    }

    //fill count array, counting how many times each index no appears in values array
    for (int i = 0; i < n; i++)
    {
        count[values[i]]++;
    }

    //increment each element by the value of the preceding element
    for (int i = 1; i < MAX; i++)
    {
        count[i] += count[i-1];
    }

    //create and fill output array
    int output[n];

    for (int i = n-1; i >= 0; i--)
    {
        count[values[i]]--;
        output[count[values[i]]] = values[i];
    }

    for (int i = 0; i < n; i++)
    {
        values[i] = output[i];
    }

    /*
    // BUBBLE SORT ALGORITHM

    int swap;
    int temp;
    int i;

    do //while swap counter is greater than 0 - which indicates we haven't done a full pass without swaps yet
    {
        //reset swap counter
        swap = 0;

        //iterate through each element in values[]
        //IMPORTANT: n is the number of elements in the array.
        //Because we are looking to the next element (i + 1) in the loop, we have to pull up short (i < n - 1) so that we don't go out of bounds
        for (i = 0; i < n - 1; i++)
        {
            if (values[i] > values[i+1])
            {
                //swap i and i+1
                temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;

                //increment swap counter to indicate we have made another swap this pass
                swap++;
            }
        }

        //at the end of each pass, the highest number will always be at the end; so we can decrease the size we are checking by 1 each time
        n--;
    }
    while (swap > 0);
    */
}
