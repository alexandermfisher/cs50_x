#include <stdio.h>
#include <cs50.h>

const int MIN_HEIGHT = 1;   //  min height of pyramid that user can be allowed to pass in
const int MAX_HEIGHT = 8;   //  max height of pyramid that user is allowed to pass in


int main(void)
{
    //  get pyramid height from user using cs50's get_iput
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < MIN_HEIGHT || height > MAX_HEIGHT);

    // loop over each row of bricks in the wall and print to console. (Start at top of the pyramid and work down.)
    for (int i = 1; i <= height; ++i)
    {
        for (int j = 1; j <= height - i; ++j)
            putchar(' ');

        for (int j = 1; j <= i; ++j)
            putchar('#');

        // complete this iteration by moving to next line in prepartion for the next row of bricks.
        putchar('\n');
    }

    return 0;
}