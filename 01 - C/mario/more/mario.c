#include <stdio.h>
#include <cs50.h>

const int MIN_HEIGHT = 1;               //  min height of pyramid that user can be allowed to pass in
const int MAX_HEIGHT = 8;               //  max height of pyramid that user is allowed to pass in
const int WALL_SEPARATION = 2;          //  space between the two sides, i.e number of single white spaces.


void print_white_space(int number);


int main(void)
{
    //  get pyramid height from user using cs50's get_iput (accepted input being in closed interval: [1,8])
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < MIN_HEIGHT || height > MAX_HEIGHT);

    // loop over each row of bricks in the wall and print to console. (Start at top of the pyramid and work down.)
    for (int i = 1; i <= height; i++)
    {

        // print initial white spcae starting off high reducing white space by one each iteration
        print_white_space(height - i);

        // print left-sides bricks in current row
        for (int j = 1; j <= i; j++)
            putchar('#');

        // print space between two sides
        print_white_space(WALL_SEPARATION);

        // print right-sides bricks in current row
        for (int j = 1; j <= i; j++)
            putchar('#');

        // complete this iteration by moving to next line in prepartion for the next row of bricks.
        putchar('\n');

    }

    return 0;
}


void print_white_space(int number)
{
    // prints white space char number times.
    for (int i = 0; i < number; ++i)
        putchar(' ');
}

