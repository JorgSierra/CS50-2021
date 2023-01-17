// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 144103;

// Hash table
node *table[N];

// Words in the dictionarie
int wordCount = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hashCode = hash(word);
    node *tmp = table[hashCode];
    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
    /*
                                     DEFINITION                                      VARIABLE
    ki : k1, k2, k3, ... , kn -> must be treated as an unsigned integer         ...   word[i]
    m : ammount of buckets -> must be a prime number                            ...     N
    h: must be initialized at 0 and then combined with XOR over all the ki's    ...     h

    CRC variant: Do a 5-bit left circular shift of h. Then XOR in ki. Specifically:

     highorder = h & 0xf8000000    // extract high-order 5 bits from h
                                   // 0xf8000000 is the hexadecimal representation
                                   //   for the 32-bit number with the first five
                                   //   bits = 1 and the other bits = 0
     h = h << 5                    // shift h left by 5 bits
     h = h ^ (highorder >> 27)     // move the highorder 5 bits to the low-order
                                   //   end and XOR into h
     h = h ^ ki

    */
    // For debug print
    /*
    char copy[46];
    strcpy(copy, word);
    */
    unsigned int h = 0, highorder;

    while (*word != '\0')
    {
        highorder = h & 0xf8000000;
        h = h << 5;
        h = h ^ (highorder << 27);
        h = h ^ (unsigned int)(tolower(*word));
        word++;
    }
    // Debug print
    // printf("%s hashed as %i\n", copy, (h % N));
    return h % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opens the file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Buffer
        node *p = malloc(sizeof(node));
        if (p == NULL)
        {
            printf("Not enough memory to alocate.\n");
            fclose(file);
            return false;
        }
        // Init buffer
        strcpy(p->word, word);
        p->next = NULL;

        // Get hash code of the word
        unsigned int hash_code = hash(word);

        // Insert the node on the corresponding linked list at the beggining
        if (table[hash_code] != NULL)
        {
            p->next = table[hash_code];
        }
        table[hash_code] = p;
        wordCount++;
    }
    fclose(file);
    // Print loaded dictionary
    /*
    node *tmp = NULL;
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            tmp = table[i];
            printf("%i.  ", i);
            while (tmp != NULL)
            {
                printf("%s.  ", tmp->word);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("Word count is:  %i.  \n", wordCount);
    */
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return wordCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp = NULL;
    int i;
    for (i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            tmp = table[i];
            while (tmp != NULL)
            {
                table[i] = tmp->next;
                free(tmp);
                wordCount--;
                tmp = table[i];
            }
        }
    }
    if (i != N)
    {
        return false;
    }
    else
    {
        return true;
    }
    // Print word count after unloaded
    //printf("\n Word count after unload is:  %i.  \n", wordCount);
}
