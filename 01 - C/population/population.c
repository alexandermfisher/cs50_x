#include <cs50.h>
#include <stdio.h>

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

    int years = 0;
    while (start_size < end_size)
    {
        start_size = start_size + start_size / 3 - start_size / 4;
        years++;
    }
    
    // Print number of years
    printf("Years: %i\n", years);
}