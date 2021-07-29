// caesar project in pset2

// import all necessary libraries:
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

const int LENGTH_ALPHABET = 26;


// declare function prototypes:
string caeser_algorithm(string plaintext, int key);



int main (int argc, string argv[])
{
    // check for key, and if incorrect print usage to console:
    int key;
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        // check all characters are digits
        for (int i = 0, n = strlen(argv[1]); i < n; ++i)
        {
            if (isdigit(argv[1][i]) == 0)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }

        key = atoi(argv[1]) % 26;
    }

    // get plaintext from user:
    string plaintext = get_string("plaintext:  ");

    // pass in text and key to algorithm and print the resultant cipher text:
    printf("ciphertext: %s\n", caeser_algorithm(plaintext, key));

    return 0;
}


string caeser_algorithm(string text, int key)
{
    // iterate over each charcater and rotate by key elements if alphabetical preserving capitalisation:
    for (int i = 0, n = strlen(text); i < n; ++i)
    {
        // check for alphabetical:
        if (isalpha(text[i]))
        {
            if (islower(text[i]))
            {
                text[i] = ((text[i] - 'a' + key) % LENGTH_ALPHABET) + 'a';
            }
            else
            {
                text[i] = ((text[i] - 'A' + key) % LENGTH_ALPHABET) + 'A';
            }
        }
    }

    return text;
}