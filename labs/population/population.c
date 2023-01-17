#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int start, end, years = 0;
    //Prompt user for start size
    do
    {
        start = get_int("Start size : ");
    }
    while (start < 9);
    //Prompt user for end size
    do
    {
        end = get_int("End size : ");
    }
    while (end < start);
    //If the population has not reach the end target calculate
    //the next years population and increase the amount of years
    while (start < end)
    {
        start += (start / 3) - (start / 4);
        years++;
    }
    //Print years result
    printf("Years: %i\n", years);

}