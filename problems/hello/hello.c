#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Propt to ask the user's name
    string name = get_string("What is your name?\n");
    //Geets the user
    printf("hello, %s\n", name);
}