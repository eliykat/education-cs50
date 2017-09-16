#include <stdio.h>
#include <cs50.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    //test we have the right number of arguments
    if (argc != 2)
    {
        printf("Incorrect usage");
        return 1;
    }

    //example crypt usage for reference
    //string pass = crypt("rofl", "50");

    int test[] = {0, 0, 0, 0};
    int i;
    char c[4];
    string hashc;
    int nchar;

    //get the salt from the first 2 characters of the hash
    char salt[2];
    sprintf(salt, "%c%c", argv[1][0], argv[1][1]);

    while (true)
    {

        //TEST NUMBERS
        sprintf(c, "%c%c%c%c", test[0], test[1], test[2], test[3]);
        hashc = crypt (c, salt);

        if ( strcmp (hashc, argv[1]) == 0 )
        {
            printf("%s\n", c);
            return 0;
        }

        //INCREMENT NUMBERS

        //set i to 3, which corresponds to the last item in the test array
        i = 3;

        while (true)
        {

        nchar = test[i] + 1;

            if ((nchar >= 'A' && nchar <= 'Z') || (nchar >= 'a' && nchar <= 'z'))
            {
                //if nchar is in the middle of the alphabet, increment by 1
                test[i]++;
                break;

            }
            else if (nchar > 0 && nchar < 'A' )
            {
                //if nchar is between nul and A, set value to A
                test[i] = 'A';
                break;
            }
            else if (nchar > 'Z' && nchar < 'a')
            {
                //if nchar is between uppercase and lowercase alphabets, set to a (to avoid the symbols)
                test[i] = 'a';
                break;
            }
            else
            {
                //then nchar is greater than z and we've hit the end of the alphabet

                test[i] = 0; //reset value of current column.
                if (i - 1 < 0)  //check that there is another column to the left
                {
                    //no more columns - can't go higher - failed to detect match
                    printf("Error: no match detected.\n");
                    return 1;
                }
                else
                {
                    i--; //decrease i by 1 to move to the next column to the left
                }
            }

        }
    }
}