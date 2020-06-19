// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table. 26 for the amount of letters in alphabet.
const unsigned int N = 26;

// Number of words loaded.
int wordsLoaded = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // First you have to run the word through the hash function.
    unsigned int hashValue = hash(word);

    // Next go to the hash table using the hash value. Iterate
    // through the linked list in that bucket searching for word.
    for (node *tmp = table[hashValue]; tmp != NULL; tmp = tmp -> next)
    {
        // Make the word lowercase. HINT: strcasecmp()
        // char lowerWord[LENGTH + 1];
        // for (int i = 0; i < strlen(word); i++)
        // {
        //     if (isalpha(word[i]))
        //     {
        //         lowerWord[i] = tolower(word[i]);
        //     }
        //     else
        //     {
        //         lowerWord[i] = word[i];
        //     }
        // }

        // Make the word at the node lowercase.
        // char lowerWordNode[LENGTH + 1];
        // for (int i = 0; i < strlen(tmp -> word); i++)
        // {
        //     if (isalpha((tmp -> word)[i]))
        //     {
        //         lowerWordNode[i] = tolower((tmp -> word)[i]);
        //     }
        //     else
        //     {
        //         lowerWordNode[i] = (tmp -> word)[i];
        //     }
        // }

        // Compare the two words.
        if (strcasecmp(word, tmp -> word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Grab the first character in word and make it lower case.
    char letter = tolower(word[0]);

    // Turn that letter into a value 0-25 or A-Z case insensitive.
    unsigned int hashValue = letter - 97;

    return hashValue;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    // Initialize hash table.
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open the dictionary file.
    FILE *dict = fopen(dictionary, "r");

    // Return false if opening file was unsuccessful.
    if (dict == NULL)
    {
        return false;
    }

    // Initialize a string with enough space for a maximum length word.
    char word[LENGTH + 1];

    // Loop through the file grabbing words until the end of file.
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Run the word through the hash function to attain the hash value.
        unsigned int hashValue = hash(word);

        // Allocated memory for a new node. Return false if allocation of memory did not work.
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        n -> next = NULL;

        // Add word to the new node.
        strcpy(n -> word, word);

        // Check to see if there are any nodes already at the hash table location.
        // If no node added at this bucket add the first one.
        if (table[hashValue] == NULL)
        {
            table[hashValue] = n;
            wordsLoaded++;
        }
        // If the bucket already has nodes there add this node to the beginning of the linked list.
        else
        {
            n -> next = table[hashValue];
            table[hashValue] = n;
            wordsLoaded++;
        }
    }

    fclose(dict);

    return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (wordsLoaded > 0)
    {
        return wordsLoaded;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate through the entire hash table freeing up memory.

    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor -> next;
            free(tmp);
        }
    }

    return true;
}
