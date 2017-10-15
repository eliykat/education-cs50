#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_KEY 27

//struct declaration
typedef struct _node
{
    char word[46];              //used to store the word at that node.  Allows for a 45 character word + NULL terminating character
    struct _node* index[MAX_KEY];    //used to store the index of 26 alphabetic characters plus 1 apostrophe, which will link to further nodes down the tree
}
node;

//function declarations
node* create(void);
int insert(char newword[]);
int search (char target[]);
void unload(node *target);
int getkey(char c);


//global variables
node *root = NULL;

int main(void)
{
    //create our root node and sanity check
    root = create();
    if (root == NULL)
    {
        return 1;
    }

    //setup array of test words
    char *strs[5] = {"foo", "bar", "bletch", "barman", "foobar"};

    for (int i = 0; i < 3; i++)
    {
        //only insert first 3
        insert(strs[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        //search all 5
        if (search(strs[i]) == 0)
        {
            printf("Found %s!\n", strs[i]);
        }
        else
        {
            printf("Did not find %s!\n", strs[i]);
        }
    }

    unload(root);
}


node* create(void)
{
    node *nd = malloc(sizeof(node));

    //initialise word to NULL
    strcpy(nd->word, "");

    //initialise index array to NULL
    for (int i = 0; i < 27; i++)
    {
        nd->index[i] = NULL;
    }

    return nd;
}


int insert(char newword[])
{
    int key = 0;

    //create pointer to traverse with
    node *ptr = root;

    for (int i = 0, len = strlen(newword); i < len; i++)
    {
        //iterate over each character, reduce it to somewhere in the 27 range key, then check whether index[key] of the current node is not null.  If it is, create.  Either way, go to next index[key]
        //when we get to i = len-1, we're at the last char and need to test whether the word is contained in node->word.

        key = getkey(newword[i]);

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
            fprintf(stderr, "Successfully written %s.\n", ptr->word);
            return 0;
        }
    }

    fprintf(stderr, "Error: reached end of insert function without writing newword.");
    return 1;

}

int search (char target[])
{
    int key = 0;

    //create pointer to traverse with
    node *ptr = root;

    for (int i = 0, len = strlen(target); i < len; i++)
    {

        key = getkey(target[i]);

        //examine pointer at key location
        if (ptr->index[key] == NULL)
        {
            //hit null without finding a match - word not found
            return 1;
        }
        else
        {
            //move pointer to next node
            ptr = ptr->index[key];
        }

        //if we are at the end of the word, check whether our node contains the word
        if (i == len - 1 && strcmp(ptr->word, target) == 0)
        {
            //found a match!
            return 0;
        }
    }

    //hit the end of the word without finding a match, even though we didn't run out of nodes
    return 1;
}


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

    //make sure we haven't been thrown any illegal characters that exceed our 27 element key
    if (key > MAX_KEY-1 || key < 0)
    {
        fprintf(stderr, "Error: Illegal character.\n");
        return 2;
    }

    return key;
}

void unload(node *target)
{
    for (int i = 0; i < MAX_KEY-1; i++)
    {
        if (target->index[i] != NULL)
        {
            unload(target->index[i]);
        }
    }

    free(target);
}