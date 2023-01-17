#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Prototypes
bool justalpha(int len, string text);
bool repited(int len, string word);
void cipher(string word, string key);

int main(int argc, string argv[])
{
    int len;
    if (argc == 2)
    {
        len = strlen(argv[1]);
    }
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (len != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (!justalpha(len, argv[1]))
    {
        printf("Key must contain ONLY alphabetic characters.\n");
        return 1;
    }
    else if (repited(len, argv[1]))
    {
        printf("Every character of the key must be unique.\n");
        return 1;
    }
    else
    {
        string plain = get_string("plaintext: ");
        cipher(plain, argv[1]);
        return 0;
    }

}
// Prints the ciphered word
void cipher(string word, string key)
{
    int position;
    printf("ciphertext: ");
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isalpha(word[i]))
        {
            if (isupper(word[i]))
            {
                position = word[i] - 'A';
                printf("%c", key[position]);
            }
            else
            {
                position = word[i] - 'a';
                printf("%c", tolower(key[position]));
            }
        }
        else
        {
            printf("%c", word[i]);
        }
    }
    printf("\n");
}

// Evaluates if there are repited characters
bool repited(int len, string word)
{
    int position;
    bool alphareg[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0;  i < len; i++)
    {
        if (isupper(word[i]))
        {
            position = word[i] - 'A';
        }
        else
        {
            position = word[i] - 'a';
            word[i] = toupper(word[i]);
        }

        if (!alphareg[position])
        {
            alphareg[position] = true;
        }
        else
        {
            return true;
        }
    }
    return false;
}
// Evaluates if there are just aphabetic characters
bool justalpha(int len, string text)
{
    for (int i = 0;  i < len; i++)
    {
        if (!isalpha(text[i]))
        {
            return false;
        }
    }
    return true;
}