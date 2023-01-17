#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    //Prompt user for height
    do
    {
        height = get_int("Height : ");
    }
    while ((height <= 0) || (height >= 9));
    
    //For every row
    for (int i = 1; i <= height; i++)
    {   
        //How many spaces first
        for (int j = 0; j < height - i; j++)
        {
            printf(" ");
        }
        //How many hashes
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        //THE 2 spaces GAP 
        for (int j = 0; j < 2; j++)
        {
            printf(" ");
        }
        //Last hash row
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }

}