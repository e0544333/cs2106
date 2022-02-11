#include <stdlib.h>
#include <stdio.h>
#include "bintree.h"

/* ------------------------- DO NOT IMPLEMENT THESE ----------------------

These are provided "free" to you, and you do not need to implement them,
though you MAY  need to understand what they do. Some give you hints
on how to work with pointers to pointers. You MAY find some of these
functions useful.

  ------------------------------------------------------------------------- */

// Searches for the node containing "name" in the tree whose root is at "root",
// returning both the node ifself in "node", and its parent in "prevnode"
// (useful for deleting node). Both "node" and "prevnode" are set to NULL
// if name is not found in the tree.

void findNode(char *name, TTreeNode *root, TTreeNode **node, TTreeNode **prevnode)
{

    TTreeNode *trav = root;
    TTreeNode *prev = NULL;

    while (trav != NULL)
    {
        int cmp = strcmp(trav->name, name);

        if (cmp == 0)
        {
            *node = trav;
            *prevnode = prev;
            return;
        }

        prev = trav;
        if (cmp < 0)
            trav = trav->right;
        else
            trav = trav->left;
    }

    *node = NULL;
    *prevnode = NULL;
}

// Searches for the node with the smallest value ("smallest node") in the tree originating at "node".
// Returns the smallest node and its parent in "smallest_node" and "parent"
// respectively.
void findSmallest(TTreeNode *node, TTreeNode **smallest_node, TTreeNode **parent)
{
    TTreeNode *trav = node;
    TTreeNode *prev = NULL;

    if (trav == NULL)
        return;

    while (trav->left != NULL)
    {
        prev = trav;
        trav = trav->left;
    }

    *smallest_node = trav;
    *parent = prev;
}

// Delete a node at "node" with parent node "prevnode"
void delNode(TTreeNode *node, TTreeNode *prevnode)
{
    // This is a leaf node
    if (node->left == NULL && node->right == NULL)
    {
        // See whether node is on parent's left or right, and NULL the
        // corresponding pointer

        int cmp = strcmp(prevnode->name, node->name);

        // Previous node is smaller than this one. This
        // node is on the right
        if (cmp < 0)
            prevnode->right = NULL;
        else
            prevnode->left = NULL;

        freenode(node);

        return;
    }

    // This has a child on the left only
    if (node->right == NULL)
    {
        // Copy the right child over
        node = node->left;
        freenode(node->left);
        return;
    }

    // This has a child on the right only
    if (node->left == NULL)
    {
        node = node->right;
        freenode(node->right);
        return;
    }

    // This has children on both nodes
    TTreeNode *smallest, *smallest_parent;
    findSmallest(node->right, &smallest, &smallest_parent);
    node = smallest;
    smallest_parent->left = NULL;
    freenode(smallest);
}

/* ---------------------------- IMPLEMENT THESE --------------------------- */

void delTree(TTreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    // Similar to tree traversal
    delTree(root->left);
    delTree(root->right);
    freenode(root);
}

TTreeNode *makeNewNode(char *name, char *phoneNum)
{
    TTreeNode *n = (TTreeNode *)malloc(sizeof(TTreeNode));
    n->name = (char *)malloc(strlen(name) + 1);
    strcpy(n->name, name);
    strcpy(n->phoneNum, phoneNum);

    n->left = NULL;
    n->right = NULL;

    return n;
}

void addNode(TTreeNode **root, TTreeNode *node)
{

    if (*root == NULL)
        *root = node;
    else
    {

        TTreeNode *trav = *root;

        while (trav != NULL)
        {
            int cmp = strcmp(trav->name, node->name);

            if (cmp < 0)
                if (trav->right == NULL)
                {
                    trav->right = node;
                    break;
                }
                else
                    trav = trav->right;
            else if (trav->left == NULL)
            {

                trav->left = node;
                break;
            }
            else
                trav = trav->left;
        }
    }
}

void freenode(TTreeNode *node)
{
    free(node->name);
    free(node);
    node = NULL;
}

void print_inorder(TTreeNode *node)
{
    if (node == NULL)
    {
        return;
    }

    print_inorder(node->left);
    printf("Name: %s phoneNumber: %s\n", node->name, node->phoneNum);
    print_inorder(node->right);
}
