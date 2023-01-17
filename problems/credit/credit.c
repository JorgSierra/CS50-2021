#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

int main(void)
{
    long ccn = 0;
    // Prompt user for credit card number
    do
    {
        ccn = get_long("Number : ");
    }
    while (ccn == 0);
    // Initialice variables just if ccn is a valid long
    long aux = ccn;
    int digits = 0, last = 0, secondtolast = 0;
    string output;
    // Count digits of the ccn
    do
    {
        aux /= 10;
        digits++;
    }
    while (aux != 0);
    // Obtains first two digits of the ccn
    int firstTwo = (int)(ccn / pow(10, digits - 2));
    // Clasifies the ccn 
    if (digits >= 13 && digits <= 16)
    {
        if (firstTwo ==  34 || firstTwo == 37)
        {
            output = "AMEX\n";
        }
        else if (firstTwo ==  51 || firstTwo == 52 || firstTwo ==  53 || firstTwo == 54 || firstTwo == 55)
        {
            output = "MASTERCARD\n";
        }
        else if (firstTwo / 10 == 4)
        {
            output = "VISA\n";
        }
        else
        {
            output = "INVALID\n";
        }
    }
    else
    {
        output = "INVALID\n";
    }
    // Verify using Luhn's Algorithm if the card is not INVALID
    if (strcmp(output, "AMEX\n") == 0 || strcmp(output, "MASTERCARD\n") == 0 || strcmp(output, "VISA\n") == 0)
    {
        for (int i = 0; i < digits; i++)
        {
            aux = ccn % (long) pow(10, i + 1);
            aux /= (long) pow(10, i);
            if (i % 2 == 0)
            {
                last += (int) aux;
            }
            else
            {
                aux *= 2;
                secondtolast += (int)(aux % 10);
                secondtolast += (int)(aux / 10);
            }
        
        }
        if (((last + secondtolast) % 10) != 0)
        {
            output = "INVALID\n";
        }
    }
    // Print the output
    printf("%s", output);
}