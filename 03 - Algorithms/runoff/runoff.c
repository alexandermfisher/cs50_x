#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // check name is valid, and if so add vote for candidate:
    for (int i = 0; i < candidate_count; ++i)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }

    // if no match was found for name then it is an invalid vote, hence return false.
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // loop through each voter, and each preference:
    for (int i = 0; i < voter_count; ++i)
    {
        for (int j = 0; j < candidate_count; ++j)
        {
            // if candidate is not elimanted add vote and break loop, so as to move on to next voter.
            if (!candidates[preferences[i][j]].eliminated)
            {
                candidates[preferences[i][j]].votes += 1;
                break;
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // search for biggest number of votes
    int max_num_votes = 0;
    for (int i = 0; i < candidate_count; ++i)
    {
        if (candidates[i].votes > max_num_votes)
        {
            max_num_votes = candidates[i].votes;
        }
    }

    // if max num votes is greater than half print winners:
    if (max_num_votes >= ((voter_count / 2) + 1))
    {
        for (int i = 0; i < candidate_count; ++i)
        {
            if (candidates[i].votes == max_num_votes)
            {
                printf("%s\n", candidates[i].name);
            }
        }

        return true;
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // loop through candidates and find min number of votes:
    int min_num_votes = voter_count;
    for (int i = 0; i < candidate_count; ++i)
    {
        if (candidates[i].votes < min_num_votes && candidates[i].eliminated == false)
        {
            min_num_votes = candidates[i].votes;
        }
    }

    return min_num_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // return false for tie if candidate is found to have differnt number of votes (i.e. != min):
    for (int i = 0; i < candidate_count; ++i)
    {
        if (candidates[i].votes != min && candidates[i].eliminated == false)
        {
            return false;
        }
    }

    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // loop through candidates and if they have min votes eliminate them:
    for (int i = 0; i < candidate_count; ++i)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}
