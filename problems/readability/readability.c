#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Prototypes
void get_data(int data[], string text);

int main(void)
{
    string text = get_string("Text: ");
    // Letters, words, sentences 
    int data [] = {0, 1, 0};

    get_data(data, text);
    float L = (data[0] * 100.0) / data[1]; // Letters * 100 / words
    float S = (data[2] * 100.0) / data[1]; // Sentences * 100 / words
    float index = 0.0588 * L - 0.296 * S - 15.8;
    index = round(index);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)index);
    }
}

void get_data(int data[], string text)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (islower(text[i]) || isupper(text[i]))
        {
            data[0]++;
        }
        if (text[i] == ' ')
        {
            data[1]++;
        }
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            data[2]++;
        }
    }
}