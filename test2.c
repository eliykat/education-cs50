#include <stdio.h>
#include <ctype.h>
#include <string.h>



void lower(char word[])
{

    for (int i = 0, len = strlen(word); i < len; i++)
    {
        word[i] = tolower(word[i]);
    }

}

int main(void)
{

    char word[] = "TESTING";

    lower(word);

    printf("%s", word);

}
