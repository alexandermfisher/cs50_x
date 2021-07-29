// Implements a dictionary's functionality

#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//number of elements considering, i.e. length of alphabet
const int LEN_ALPHABET = 26;

// Number of buckets in hash table
const unsigned int N = 26*26*26;

// Number of elements in hash table (i.e words in dictionary)
int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // search table to see if in dictionary ignoring case
    for (node *tmp = table[hash(word)]; tmp != NULL; tmp = tmp->next) {

        if (strcasecmp(tmp->word, word) == 0) {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 401;     // initial shift to enhance distibution of elements in buckets
    int mul = 1;
    for (int i = 0, n = strlen(word); i < n; ++i) {
        int letter  = tolower(word[i]);
        letter -= 'a';
        mul = (i % 3 == 0) ? 1 : mul * LEN_ALPHABET;
        sum += letter * mul;
    }

    int hash_value = sum % N;

    return hash_value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL){
        printf("Failed to open file\n");
        return false;
    }

    // initialise hash table to contain NULL pointers
    for (int i = 0; i < N; ++i) {
        table[i] = NULL;
    }

    // read in word by word and store in hash table
    char dic_word[LENGTH + 1];
    while (fscanf(file, "%s", dic_word) != EOF) {

        // store word in hash table accordingly by inserting at beginning of linked list
        node *n = malloc(sizeof(node));
        if (n == NULL) {
            unload();
            fprintf(stderr, "Failed to allocate enough memory for storing node.");
            return false;
        }
        strcpy(n->word, dic_word);
        int hash_value = hash(dic_word);
        n->next = table[hash_value];
        table[hash_value] = n;
        word_count++;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// frees memory for a linked list starting at list
void free_linked_list(node *list) {

    // check to see if end is reached
    if (list == NULL) {
        return;
    }
    // recursively free each node in the linked list
    free_linked_list(list->next);
    free(list);

    return;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // loop over buckets and free each nodein linked list
    for (int i = 0; i < N; ++i) {
        free_linked_list(table[i]);
    }
    return true;
}


