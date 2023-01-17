#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max number of pairs
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of pairs
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(int len, pair arr[]);
void lock_pairs(void);
void print_winner(void);
bool isCicle(int cicleStart, int current);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [pair ...]\n");
        return 1;
    }

    // Populate array of pairs
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of pairs is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for winner
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
        printf("\n");
    }

    add_pairs();
    sort_pairs(pair_count, pairs);
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 1; j < (candidate_count - i); j++)
        {
            preferences[ranks[i]][ranks[i + j]]++;
        }
    }
    return;
}

// Record pairs of pairs where one is preferred over the other -- Index
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 1; j < (candidate_count - i); j++)
        {
            if (preferences[i][j + i] > preferences[j + i][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j + i;
                pair_count++;
            }
            else if (preferences[i][j + i] != preferences[j + i][i])
            {
                pairs[pair_count].winner = j + i;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory -- Total Number of voters who pefered
void sort_pairs(int len, pair arr[])
{
    // If only one number
    //      Quit
    // Else
    //      mergeSort left half
    //      mergeSort rigth half
    //      MERGE sorted halfs

    if (len == 1)
    {
        return;
    }
    else
    {
        // Calculate left and right length
        int left = round(len / 2.0);
        int right = len - left;

        // Populate the left half
        pair leftHalf [left];
        for (int i = 0; i < left; i++)
        {
            leftHalf[i] = arr[i];
        }
        // Evaluate for left half
        sort_pairs(left, leftHalf);

        // Populate right half
        pair rightHalf [right];
        for (int i = 0; i < right; i++)
        {
            rightHalf[i] = arr[left + i];
        }
        // Evaluate for right half
        sort_pairs(right, rightHalf);

        // -------------
        // --- MERGE ---
        // -------------
        int countLeft = 0, countRight = 0;
        for (int i = 0; i < (left + right); i++)
        {
            if (countLeft < left && countRight < right)
            {
                // SORTING CONDITION
                if (preferences[leftHalf[countLeft].winner][leftHalf[countLeft].loser] >
                    preferences[rightHalf[countRight].winner][rightHalf[countRight].loser])
                {
                    arr[i] = leftHalf[countLeft];
                    countLeft++;
                }
                else
                {
                    arr[i] = rightHalf[countRight];
                    countRight++;
                }
            }
            else if (countLeft < left)
            {
                arr[i] = leftHalf[countLeft];
                countLeft++;
            }
            else if (countRight < right)
            {
                arr[i] = rightHalf[countRight];
                countRight++;
            }
        }
        // -------------
        // -------------
    }
}

// Lock pairs into the pair graph in order, without creating cycles
void lock_pairs(void)
{
    // All pairs are sorted
    // Go trough pairs and add the pointing arrows from i to j in locked array
    // How to know there is not a cycle?
    int cicleStart, current = 255;
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        cicleStart = pairs[i].winner;
        if (isCicle(cicleStart, current))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// HELPER to know if an the arrow that has been set generates a cicle
bool isCicle(int cicleStart, int current)
{
    if (cicleStart == current)
    {
        return true;
    }
    else
    {
        if (current == 255)
        {
            current = cicleStart;
        }
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[current][i])
            {
                if (isCicle(cicleStart, i))
                {
                    return true;
                }
            }
        }
        return false;
    }


}

// Print the winner of the election
void print_winner(void)
{
    bool flag = false;
    for (int j = 0; j < candidate_count; j++)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            flag |= locked[i][j];
        }
        if (flag)
        {
            flag = false;
        }
        else
        {
            printf("%s\n", candidates[j]);
            return;
        }
    }
    return;
}


/* PRINT helpers

for (int i = 0; i < pair_count; i++)
    {
        printf("%i. Winner: %i   Loser: %i\n", i, pairs[i].winner, pairs[i].loser);
    }

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < pair_count; i++)
    {
        printf("%i. Winner: %i   Loser: %i\n", i, pairs[i].winner, pairs[i].loser);
    }

    for (int Z = 0; Z < candidate_count; Z++)
        {
            for (int K = 0; K < candidate_count; K++)
            {
                printf("%d ", locked[Z][K]);
            }
            printf("\n");
        }
        printf("\n\n");

*/