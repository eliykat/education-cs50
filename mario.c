#include <stdio.h>
#include <cs50.h>

void prchars(int n, char c);

int main(void)
{

    int height = 24;

    while (height > 23 || height < 0)
    {
        printf("Pyramid height: ");
        height = get_int();
    }

    int ihash = 1;
    int ispace = height - 1;

    for (int i = 0; i < height; i++)
    {
        prchars(ispace, ' ');
        prchars(ihash, '#');

        printf("  ");
        prchars(ihash, '#');

        printf("\n");

        ispace--;
        ihash++;
    }


}

void prchars(int n, char c)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}