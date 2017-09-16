#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    //Get the user's name
    printf("Name: ");
    string name = get_string();
    printf("\n");

    //Iterate through the name string
    for (int i = 0, len = strlen(name); i < len; i++)
    {
        //check we are at the start of a word - indicated by the last char being a space (but not this one), or by i == 1
        if (name[i] != ' ' && (name[i-1] == ' ' || i == 0))
        {
            printf("%c", toupper(name[i]));
        }
    }

    printf("\n");

}