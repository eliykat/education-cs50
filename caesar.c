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

    printf("plaintext:");
    string plaintext = get_string();

    printf("ciphertext:");

    int key = atoi(argv[1]);

    //Iterate through plaintext string
    for (int i = 0, len = strlen(plaintext); i < len; i++)
    {

        //Check case of character
        if (isupper(plaintext[i]))
        {
            //Character is uppercase
            printf("%c", ((plaintext[i] - 'A' + key) % ALPHABET_SIZE) + 'A');
        }
        else if (islower(plaintext[i]))
        {
            //Character is lowercase
            printf("%c", ((plaintext[i] - 'a' + key) % ALPHABET_SIZE) + 'a');
        }
        else
        {
            //It's a space or other character, so just print it directly
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
}