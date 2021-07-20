// credit project in pset1

#include <stdio.h>
#include <cs50.h>

const int AMEX_LENGTH = 15;             // american express credit card number length.
const int MC_LENGTH = 16;
const int VISA_LENGTH_1 = 13;
const int VISA_LENGTH_2 = 16;
const int AMEX_34 = 34;
const int AMEX_37 = 37;
const int MC_51 = 51;
const int MC_52 = 52;
const int MC_53 = 53;
const int MC_54 = 54;
const int MC_55 = 55;
const int VISA = 4;

int card_is_valid(long card_number, int number_of_digits);
int count_digits(long number);
int get_starting_digits(long number);

int main(void)
{
    // get credit card number from user:
    long card_number = get_long("Number: ");

    // get number of digits in card number:
    int number_of_digits = count_digits(card_number);

    // calculate checksum to see if card number is valid, and if so find type of card:
    if (card_is_valid(card_number, number_of_digits))
    {
        int n = get_starting_digits(card_number);

        // check if card is American Express:
        if ((number_of_digits == AMEX_LENGTH) && (n == AMEX_34 || n == AMEX_37))
        {
            printf("AMEX\n");
        }
        // check if card is MASTERCARD:
        else if ((number_of_digits == MC_LENGTH) && (n == MC_51 || n == MC_52 || n == MC_53 || n == MC_54 || n == MC_55))
        {
            printf("MASTERCARD\n");
        }
        // check if card is visa:
        else if ((number_of_digits == VISA_LENGTH_1 || number_of_digits == VISA_LENGTH_2) && ((n / 10) % 10 == VISA))
        {
            printf("VISA\n");
        }
        // if no match is found card number is invalid:
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}


int count_digits(long number)
{
    // counts number of digits in number:
    int count = 0;
    while (number > 0)
    {
        count++;
        number /= 10;
    }

    return count;
}


int card_is_valid(long card_number, int number_of_digits)
{
    // work through 2-digits at at time starting from right adding to final sum after applying the appropriate math
    // (note if number of digits is an odd number one digit will still be left to processed.)
    int sum = 0;
    for (int i = 0; i < (number_of_digits) / 2; ++i)
    {
        sum += card_number % 10;
        card_number /= 10;

        int temp = (card_number % 10) * 2;
        if (temp > 9)
        {
            // if extracted digit * 2 is in [10,18]. To add both digits to sum add 1 and remainder.
            sum += 1;
            sum += temp % 10;
            card_number /= 10;
        }
        else
        {
            // else extracted digit * 2 is in [0,9] and just add remainder to sum.
            sum += temp % 10;
            card_number /= 10;
        }

    }

    // remove last digit from card_number (if one remains) and add to sum
    if (number_of_digits % 2 != 0)
    {
        sum += card_number;
    }

    // check if card number is valid by checking last digit.
    if ((sum % 10) == 0)
    {
        // return 1 (true) as card is valid:
        return 1;
    }
    else
    {
        return 0;
    }

}


int get_starting_digits(long number)
{
    // returns the two first digits of the number:
    while (number >= 100)
    {
        number/= 10;
    }

    return number;
}