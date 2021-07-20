#include <stdio.h>
#include <cs50.h>

const int MIN_HEIGHT = 1;   //  min height of pyramid that user can be allowed to pass in
const int MAX_HEIGHT = 8;   //  max height of pyramid that user is allowed to pass in
const int SPACES = 2;       //  space between the two sides, i.e number of single white spaces.
const int SECTIONS = 4;     //  number of distinct/alternating sections that describe a row.
                            //  (e.g. space then bricks then space then bricks for this spec of mario)


int print_pyramid(int height,  int count);

int main(void)
{
    //  get pyramid height from user using cs50's get_iput
    int height = 0;             //  declare int varibable named height and initialise at invalid height value to enter loop

    //  continuosly ask for input only breaking once user inputs a number between 1 and 8 inclusive.
    do
    {
        height = get_int(" Please input a pyramid height in the range 1 to 8 inclusive: ");
    }
    while (height < MIN_HEIGHT || height > MAX_HEIGHT);

    //  initialise count to 0 (i.e. 0 rows of the pyramid have been printed)
    int count = 0;

    //  call print_pyramid which recursivley calls intself to print a row until count condition is met.
    print_pyramid(height, count);

    return 0;
}


void print_bricks(int brick_count[], int format[]);

int print_pyramid(int height,  int count)
{
    //  stop printing rows of bricks if total/corrent number has already been printed
    if (count == height)
    {
        return 0;
    }
    //  print next row of pyramid and recurslively call next print_pricks() with updated values.
    else
    {
        //  initialise a brick array which will contain how many elements (a specifc char)
        //  are to be printed in each 4 respective sections for the current row  of bricks to be printed.
        int bricks[SECTIONS];

        bricks[0] = height - 1 - count;
        bricks[1] = 1 + count;
        bricks[2] = SPACES;
        bricks[3] = 1 + count;
        //bricks[4] = height - 1 - count;

        // initialise a format array which specifies the specific char to be printed
        // in each of the corresponding 4 sections, and in doing so to complete the "row of bricks definition" aspect.
        int format[SECTIONS];

        format[0] = ' ';
        format[1] = '#';
        format[2] = ' ';
        format[3] = '#';
        //format[4] = ' ';

        // print next row of bricks and move down one row/line
        print_bricks(bricks, format);
        putchar('\n');

        // update count and pass into print pyramid to complete next step of the printing process.
        count += 1;
        print_pyramid(height, count);
    }

    return 0;
}


// given brick (or a lack thereof) counts for each section
// and  the format of each corresponfing char the row is printed from left to right.
void print_bricks(int brick_count[], int format[])
{
    // for the 4 sections of the brick (defining in this context a layout of a
    // mario wall row of bricks generically) print correct number of given char
    for (int i = 0; i < SECTIONS ; ++i)
    {
        for (int j = 0; j < brick_count[i]; ++j)
        {
            putchar(format[i]);
        }
    }

}
