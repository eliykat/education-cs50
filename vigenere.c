#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

int main(int argc, string argv[])
{
    //Check we have the right number of arguments
    if (argc != 2)
    {
        printf("Error: you must provide the key as a command line argument.\n");
        return 1;
    }


    //declare int key array, equal to the length of the string key entered by the user
    int key_len = strlen(argv[1]);
    int key[key_len];

    //Convert string key into array of ints
    for (int i = 0; i < key_len; i++)
    {
        //check i'th char of string key is alpha
        if (isalpha(argv[1][i]))
        {
            key[i] = toupper(argv[1][i]) - 'A';
        }
        else
        {
            printf("Error: key must be alphanumeric\n");
            return 1;
        }
    }

    printf("plaintext:");
    string plaintext = get_string();

    printf("ciphertext:");

    //Iterate through plaintext string
    for (int i = 0, j = 0, text_len = strlen(plaintext); i < text_len; i++)
    {

        //check the i'th char is a letter
        if (isalpha(plaintext[i]))
        {
            //check the i'th char is upper
            if (isupper(plaintext[i]))
            {
                printf("%c", ((plaintext[i] - 'A' + key[j]) % ALPHABET_SIZE) + 'A');
            }
            else
            {
                printf("%c", ((plaintext[i] - 'a' + key[j]) % ALPHABET_SIZE) + 'a');
            }

            //check whether incrementing j would exceed the bounds of the key array
            if (j + 1 > key_len -1)
            {
                //restart the cycle through the key array
                j = 0;
            }
            else
            {
                j++;
            }
        }
        else //i'th char is not a letter, just reprint it
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}