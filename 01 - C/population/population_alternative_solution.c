#include <cs50.h>
#include <stdio.h>


int years_to_update_population(int start_size, int end_size, int years);

int main(void)
{
    int start_size;    // declare varibale for starting population size
    int end_size;      // declare variable for ending population size

    // Prompt for start size
    do
    {
        start_size = get_int("Start Size: ");
    }
    while (start_size < 9);

    // Prompt for end size
    do
    {
        end_size = get_int("End Size: ");
    }
    while (end_size < start_size);
    
    // Calculate number of years until we reach threshold
    int years = years_to_update_population(start_size, end_size, 0);
    
    // Print number of years
    printf("Years: %i\n", years);
}


int years_to_update_population(int start_size, int end_size, int years)
{
    // if desired population size achieved break out and return years taken
    if (start_size >= end_size)
    {
        return years;
    }
    // pass in updated values recursivley until end_size has been achieved.
    else
    {   
        int updated_population = start_size + start_size / 3 - start_size / 4;
        years++;
        return years_to_update_population(updated_population, end_size, years);
    }
}