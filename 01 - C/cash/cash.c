// cash project in pset1

#include <stdio.h>
#include <cs50.h>
#include <math.h>

const int QUARTER = 25;     // set quarters to 25 (in pennies)
const int DIME = 10;        // set dimes to 10 (in pennies)
const int NICKEL = 5;       // set nickels to 5 (in pennies)
const int PENNY = 1;        // set penny to 1 (in pennies)



void get_next_coin(int *current_change, int *coin_count);

int main(void)
{
    // get change owed from user in dollars:
    float change_in_dollars;
    do
    {
        change_in_dollars = get_float("Change owed: ");
    }
    while (change_in_dollars < 0);

    // reformat change to purely pence and declare a coint_count var for counting total number of coins used to generate the change.
    int change_in_cents =  round(change_in_dollars * 100.0);
    int coin_count = 0;

    // reduce current change (greedily) and loop throuh until there is no change left to process
    do
    {
        // get next coin and update change_in_pence accordingly
        get_next_coin(&change_in_cents, &coin_count);
    }
    while (change_in_cents > 0);

    printf("%i", coin_count);
    putchar('\n');

    return 0;
}

void get_next_coin(int *current_change, int *coin_count)
{
    if (*current_change >= QUARTER)
    {
        *coin_count += *current_change / QUARTER;
        *current_change = *current_change - ((*current_change / QUARTER) * QUARTER);
    }
    else if (*current_change >= DIME)
    {
        *coin_count += *current_change / DIME;
        *current_change = *current_change - ((*current_change / DIME) * DIME);
    }
    else if (*current_change >= NICKEL)
    {
        *coin_count += *current_change / NICKEL;
        *current_change = *current_change - ((*current_change / NICKEL) * NICKEL);
    }
    else
    {
        *coin_count += *current_change / PENNY;
        *current_change = *current_change - ((*current_change / PENNY) * PENNY);
    }
}