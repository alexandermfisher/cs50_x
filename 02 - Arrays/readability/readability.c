// readability project in pset2

// import all necessary libraries:
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// declare function prototypes:
void count_letters_words_sentences(string text, int *letter_count, int *word_count, int *sentence_count);
int get_coleman_liau_index(int letter_count, int word_count, int sentence_count);


int main(void)
{
    // get text from user:
    string text = get_string("Text: ");

    // count letters, words, and sentences:
    int letter_count = 0, word_count = 0, sentence_count = 0;
    count_letters_words_sentences(text, &letter_count, &word_count, &sentence_count);

    // calculate the index from the counts, and print result to console:
    int index = get_coleman_liau_index(letter_count, word_count, sentence_count);

    if (index > 0 && index < 16)
    {
        printf("Grade %i\n", index);
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade 16+\n");
    }

    return 0;
}


void count_letters_words_sentences(string text, int *letter_count, int *word_count, int *sentence_count)
{
    // add letter if alphabetical, add word when a space is seen, add sentence if . or ! or ? is seen.
    for (int i = 0, n = strlen(text); i < n; ++i)
    {
        if (isalpha(text[i]))
        {
            *letter_count += 1;
        }
        else if (isspace(text[i]))
        {
            *word_count += 1;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            *sentence_count += 1;
        }
    }
    // add one to word count to account for the final word that had no space afterwards:
    *word_count += 1;
}


int get_coleman_liau_index(int letter_count, int word_count, int sentence_count)
{
    float L, S;
    L = (letter_count * 100.0) / word_count;
    S = (sentence_count * 100.0) / word_count;

    // return coleman_liau index rounded to nearest integer value:
    return round(0.0588 * L - 0.296 * S - 15.8);
}