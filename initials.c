#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    //Get the user's name
    printf("Name: ");
    string name = get_string();
    printf("\n");

    //Get the length of the string
    int name_length = strlen(name);

    //Iterate through the name string
    for (int i = 0; i < name_length; i++)
    {
        //test that the current space is not blank AND the previous space was blank (or we're at the start of the word) - because this indicates the start of a word

        if (name[i] != ' ' && (name[i-1] == ' ' || i == 0))
        {

            if (name[i] > 'a' && name[i] < 'z')
            {
                printf("%c", name[i] - 32);
            }
            else
            {
                printf("%c", name[i]);
            }

        }

    }

    printf("\n");

}