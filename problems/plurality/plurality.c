#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
void mergeSort(int len, candidate arr[]);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // sort by vote number
    mergeSort(candidate_count, candidates);
    // Look if there is a tie
    int tieCount = 1;
    do
    {
        printf("%s\n", candidates[(candidate_count - tieCount)].name);
        tieCount++;
    }
    while (candidates[(candidate_count - tieCount)].votes == candidates[(candidate_count - tieCount + 1)].votes);

    return;
}

void mergeSort(int len, candidate arr[])
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
        candidate leftHalf [left];
        for (int i = 0; i < left; i++)
        {
            leftHalf[i] = arr[i];
        }
        // Evaluate for left half
        mergeSort(left, leftHalf);

        // Populate right half
        candidate rightHalf [right];
        for (int i = 0; i < right; i++)
        {
            rightHalf[i] = arr[left + i];
        }
        // Evaluate for right half
        mergeSort(right, rightHalf);

        // MERGE
        int countLeft = 0, countRight = 0;
        for (int i = 0; i < (left + right); i++)
        {
            if (countLeft < left && countRight < right)
            {
                if (leftHalf[countLeft].votes < rightHalf[countRight].votes)
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
    }
}