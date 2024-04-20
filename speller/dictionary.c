// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
int amount = 0;
FILE *fptr;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *cursor = table[hash(word)];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    /*int ascii_value = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        ascii_value = ascii_value + word[i];
    }
    return ascii_value;*/
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    fptr = fopen(dictionary, "r");
    int hashvalue;
    if (!fptr)
    {
        return false;
    }
    char str[256];
    while (fgets(str, sizeof(str), fptr))
    {
        amount++;
        hashvalue = hash(str);
        str[strlen(str) - 1] = '\0';
        node *tmp;

        // Creates a linked list if one wasn't already created
        if (table[hashvalue] == NULL)
        {
            if ((table[hashvalue] = malloc(sizeof(node))) == NULL)
            {
                return false;
            }
            table[hashvalue]->next = NULL;
            strcpy(table[hashvalue]->word, str);
            continue;
        }
        // Creates a new node if a linked list was already created
        else
        {
            if ((tmp = malloc(sizeof(node))) == NULL)
            {
                return false;
            }
            tmp->next = table[hashvalue];
            table[hashvalue] = tmp;
            strcpy(table[hashvalue]->word, str);
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return amount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor;
    node *tmp;
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        cursor = table[i];
        tmp = cursor;
        while (tmp != NULL)
        {
            tmp = tmp->next;
            free(cursor);
            cursor = tmp;
        }
    }
    fclose(fptr);
    return true;
}
