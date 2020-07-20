 // Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

//Dictoinary word counter
long word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int hash_value = hash(word);
    node *temp = table[hash_value];

    while (temp != NULL)
    {
        if (strcasecmp(temp->word, word) == 0)
        {
            return true;
        }
        else
        {
            temp = temp->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)  //Hash function taken from https://cs50.stackexchange.com/questions/37209/pset5-speller-hash-function
{
    int length = strlen(word);
    char lowercase[length];
    for (int g = 0; g < length; g++)
    {
        lowercase[g] = tolower(word[g]);
    }

    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++)
    {
        hash_value = (hash_value << 2) ^ lowercase[i];
    }
    return hash_value % N; //N is size of hashtable
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r"); //Open dictionary
    if (!dict) //Check if opened correctly
    {
        return false;
    }
    else
    {
        char temp_word[LENGTH + 1];

        while (fscanf(dict, "%s", temp_word) != EOF)
        {
            node *new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                printf("Error creating new node");
                free(new_node);
                return 1;
            }

           strcpy(new_node->word, temp_word);
            new_node->next = NULL;
            int hash_value = hash(temp_word);

            if (table[hash_value] == NULL)
            {
                table[hash_value] = new_node;
            }
            else
            {
                new_node->next = table[hash_value];
                table[hash_value] = new_node;
            }
            word_count++;
        }
        fclose(dict);
        return true;
    }
    fclose(dict);
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *temp = cursor;

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }
        free(cursor);
    }
    return true;
}
