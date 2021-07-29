// substitution project in pset2

// import all necessary libraries:
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

const int LENGTH_ALPHABET = 26;


// declare function prototypes:
int valid_key(string key);
string perform_substitution(string text, string key);

int main(int argc, string argv[])
{
    // check for key, and if incorrect print usage to console:
    string key;
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
    {
        // check input is valid:
        if (valid_key(argv[1]))
        {
            key = argv[1];
        }
        else
        {
            return 1;
        }
    }


    // get plaintext from user:
    string plaintext = get_string("plaintext:  ");

    // pass in text and key to algorithm and print the resultant cipher text:
    printf("ciphertext: %s\n", perform_substitution(plaintext, key));

    return 0;
}

int valid_key(string key)
{
    // check that the string len is 26.
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 0;
    }

    // initialise all letters to 0 where index corresponds to a letter i.e. [0,25] represents [a,z].
    int letter_counter[LENGTH_ALPHABET];
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        letter_counter[i] = 0;
    }

    // iterate over each char in key (same length of alphabet)
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        // check for non alphabetical letters, exit if one is found:
        if (isalpha(key[i]) == 0)
        {
            printf("Key must not contain non-alphabetical characters.\n");
            return 0;
        }
        // add one to a counter each time a specific letter is observed:
        else
        {
            letter_counter[tolower(key[i]) - 'a'] += 1;
        }
    }

    // check each letter only appears once:
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        if (letter_counter[i] != 1)
        {
            printf("Key must only contain each letter in the alphabet once. All letters must be included in the key aswell.\n");
            return 0;
        }
    }

    // all checks have been passed return 1 as key is valid:
    return 1;
}


string perform_substitution(string text, string key)
{
    // iterate over each charcater and replace with key element if alphabetical preserving capitalisation:
    for (int i = 0, n = strlen(text); i < n; ++i)
    {
        // check for alphabetical:
        if (isalpha(text[i]))
        {
            if (islower(text[i]))
            {
                text[i] = tolower(key[text[i] - 'a']);
            }
            else
            {
                text[i] = toupper(key[text[i] - 'A']);
            }
        }
    }

    return text;
}

