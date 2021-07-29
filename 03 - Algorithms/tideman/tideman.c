#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
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

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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

    // Query for votes
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
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // check if name matches a candidate and if so add to ranks[] accordingly
    for (int i = 0; i < candidate_count; ++i)
    {
        if (strcmp(name, candidates[i]) == 0)
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
    for (int i = 0; i < candidate_count; ++i)
    {
        for (int j = 1 + i; j < candidate_count; ++j)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // loop through each of the pairs of candidates and compare differnce in prefrences to add winner/loser pairs.
    for (int i = 0; i < candidate_count; ++i)
    {
        for (int j = i; j < candidate_count; ++j)
        {
            if ((preferences[i][j] - preferences[j][i]) > 0)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if ((preferences[i][j] - preferences[j][i]) < 0)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    // use bubble sort to from highest to lowest
    int changes;
    do
    {
        changes = 0;
        for (int i = 0; i < pair_count - 1; ++i)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i+1].winner][pairs[i+1].loser])
            {
                pair lower = pairs[i];
                pair higher = pairs[i+1];
                pairs[i] = higher;
                pairs[i+1] = lower;
                changes++;
            }
        }
    } while (changes > 0);

    return;
}


bool check_for_cycle(int to, int starting_candidate);

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // loop over all pairs and lock if no cycle is created by adding current pair.
    for (int i = 0; i < pair_count; ++i)
    {
        // lock the pair in and check for cycle. If a cycle occurs remove last edge i.e unlock last pair.
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (check_for_cycle(pairs[i].loser, pairs[i].winner) == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}

// check for cycle in graph by following graph from starting point to end
// if starting candidate reappears cycle exists, else not.
bool check_for_cycle(int to, int starting_candidate)
{
    // check current 'to' candidate is not the starting candidate
    // if it is then a cycle exists in the graph and return true.
    if (to == starting_candidate)
    {
        return true;
    }
    // check all children nodes of 'to' do not lead to a cycle as well.
    for (int i = 0; i < candidate_count; ++i)
    {
        // check to see if candidate 'i' is a child node of 'to'
        if (locked[to][i] == true)
        {
            if (check_for_cycle(i, starting_candidate) == true)
            {
                return true;
            }
        }
    }
    return false;
}


// Print the winner of the election. As it can be assumed there is only one winner
// find first candidate to have all falses in col. of locked i.e (no one points to him in the directed graph.).
void print_winner(void)
{
    bool winner;
    for (int i = 0; i < candidate_count; ++i)
    {
        winner = true;
        for (int j = 0; j < candidate_count; ++j)
        {
            if (locked[j][i] == true)
            {
                winner = false;
            }
        }
        if (winner == true)
        {
            printf("%s\n", candidates[i]);
        }

    }
    return;
}

