/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"





//global variables
node *root = NULL;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int key = 0;

    //create pointer to traverse with
    node *ptr = root;

    for (int i = 0, len = strlen(word); i < len; i++)
    {
        key = getkey(word[i]);

        //make sure we haven't been thrown any illegal characters that exceed our 27 element key
        if (key > MAX_KEY-1 || key < 0)
        {
            fprintf(stderr, "Error: Illegal character.\n");
            return false;
        }

        //examine pointer at key location
        if (ptr->index[key] == NULL)
        {
            //hit null without finding a match - word not found
            return false;
        }
        else
        {
            //move pointer to next node
            ptr = ptr->index[key];
        }

        //check we are at the end of the word
        if (i == len - 1)
        {
            //compare each character individually - done this way so we can use tolower() on the word
            //(not necessary for the node's word because that is already stored lowercase)
            for (int i = 0; i < len; i++)
            {
                if (tolower(word[i]) != ptr->word[i])
                {
                    return false;
                }
            }

            //found a match!
            return true;
        }
    }

    //hit the end of the word without finding a match, even though we didn't run out of nodes
    return false;

}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //open dictionary
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }

    //create our root node and sanity check
    root = create();
    if (root == NULL)
    {
        return false;
    }

    //string to store each word read from the dictionary
    char word[LENGTH+1];

    while (fscanf(fp, "%s", word) > 0)
    {
        if (insert(word) != 0)
        {
            //a word has not been inserted into our search tree, abort and return false
            return false;
        }
    }

    fclose(fp);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (root == NULL)
    {
        //dictionary is not loaded - return 0 as per specification
        return 0;
    }

    return countnodes(root);

}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    killnode(root);

    return true;
}



//custom functions

/**
 * Converts a character into a key on the 0 - MAX_KEY range.
 */
int getkey(char c)
{
    int key = 100;

    //set key value
    if (c == '\'')
    {
        key = 26;
    }
    else
    {
        key = tolower(c)-'a';
    }

    return key;
}

/**
 * Creates a new node.  Returns a pointer to the newly created node.
 */
node* create(void)
{
    node *nd = malloc(sizeof(node));

    //initialise word to NULL
    strcpy(nd->word, "");

    //initialise index array to NULL
    for (int i = 0; i < MAX_KEY; i++)
    {
        nd->index[i] = NULL;
    }

    return nd;
}

/**
 * Inserts a word into our search tree using create().  Returns 0 if successful else returns > 0.
 */
int insert(char newword[])
{
    int key = 0;

    //create pointer to traverse with
    node *ptr = root;

    //convert newword to lowercase.  pset5 presumes that all dictionary words are lowercase, but just making sure.
    for (int i = 0, len = strlen(newword); i < len; i++)
    {
        newword[i] = tolower(newword[i]);
    }

    for (int i = 0, len = strlen(newword); i < len; i++)
    {
        //transform current char into a numeric key
        key = getkey(newword[i]);

        //make sure we haven't been thrown any illegal characters that exceed our 27 element key
        if (key > MAX_KEY-1 || key < 0)
        {
            fprintf(stderr, "Error: Illegal character.\n");
            return 2;
        }

        //examine pointer at key location.  If null, create new node there
        if (ptr->index[key] == NULL)
        {
            ptr->index[key] = create();

            if (ptr->index[key] == NULL)
            {
                fprintf(stderr, "Error: could not create new node.\n");
                return 1;
            }
        }

        //move pointer to new node
        ptr = ptr->index[key];

        //check if we are on the last letter - if so, store the new word in this newly created node
        if (i == len - 1)
        {
            strcpy(ptr->word, newword);
            return 0;
        }
    }

    fprintf(stderr, "Error: reached end of insert function without writing newword.");
    return 1;

}

void killnode(node *target)
{
    for (int i = 0; i < MAX_KEY; i++)
    {
        if (target->index[i] != NULL)
        {
            killnode(target->index[i]);
        }
    }

    free(target);
}

int countnodes(node *target)
{

    int count = 0;

    //add the current node to the count if it is not empty
    if (strcmp(target->word, "") !=0)
    {
        count += 1;
    }

    //add the all the index nodes to the count
    for (int i = 0; i < MAX_KEY; i++)
    {
        if (target->index[i] != NULL)
        {
            count += countnodes(target->index[i]);
        }
    }

    return count;
}
