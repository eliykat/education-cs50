#include <stdio.h>
#include <cs50.h>

string getcompany(long long ccno);
int checkccno(long long ccno);

int main(void)
{

    printf("Input credit card number: ");
    long long ccno = get_long_long();

    if (checkccno(ccno) == 1)
    {
        printf("%s\n", getcompany(ccno));
    }
    else
    {
        printf("INVALID\n");
    }
}


int checkccno(long long ccno)
{

    int oneno = 0;
    int twono = 0;
    int digicount = 0;

    while (ccno > 0)
    {
        /*add the first digit from the right to oneno variable*/
        oneno += ccno % 10;

        /*and then permanently remove that digit from the ccno */
        ccno /= 10;
        digicount++;

        /*take the first digit from the right, multiply it by two*/
        int tempmod = (ccno % 10) * 2;

        /*if it is greater than 9, then we need to break it down further.  If not, we can add it directly.*/
        if (tempmod > 9)
        {
            twono += tempmod % 10;
            twono += tempmod / 10;
        }
        else
        {
            twono += tempmod;
        }

        /*and then permanently remove this digit from the ccno */
        ccno /= 10;
        digicount++;
    }

    digicount--;

    if (digicount == 15 || digicount == 16 || digicount == 13)
    {
        /*ccno has the right number of digits, so finish the sum*/

        if ((oneno + twono) % 10 == 0)
        {
            return 1;

        }
        else
        {
            return 0;
        }
    }
    else
    {
        /*ccno does not have right number of digits, don't bother finishing the sum - it is invalid*/
        return 0;
    }

}

string getcompany(long long ccno)
{
    while ( ccno > 99)
    {
        ccno /= 10;
    }

        switch(ccno)
    {

        case 34:
        case 37 :
            return "AMEX";

        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
            return "MASTERCARD";

        default:
            ccno /= 10;
            if (ccno == 4)
            {
                return "VISA";
            }
            else
            {
                return "INVALID";
            }
    }
}