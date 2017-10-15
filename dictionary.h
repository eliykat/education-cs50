/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define MAX_KEY 27

typedef struct _node
{
    char word[LENGTH+1];              //used to store the word at that node.  Allows for a 45 character word + NULL terminating character
    struct _node* index[MAX_KEY];    //used to store the index of 26 alphabetic characters plus 1 apostrophe, which will link to further nodes down the tree
}
node;


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);


//Custom function declarations

/**
 * Creates a new node.  Returns a pointer to the newly created node.
 */
node* create(void);

/**
 * Converts a character into a key on the 0 - MAX_KEY range.
 */
int getkey(char c);

/**
 * Inserts a word into our search tree using create().  Returns 0 if successful else returns > 0.
 */
int insert(char newword[]);

/**
 * Recursively kills a node.
 */
void killnode(node *target);

/**
 * Recursively counts words contained in each node.
 */
int countnodes(node *target);


#endif // DICTIONARY_H
