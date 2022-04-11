#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length
int mem_left = MEMSIZE;
// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr)
{
    if (ptr == NULL)
        return -1;
    else
        return (long)((char *)ptr - &_heap[0]);
}

void print_node(TNode *node)
{
    if (node != NULL && node->pdata->is_occupied == 1)
    {
        printf("Status: ALLOCATED Start index: %d Length: %ld\n",
               node->pdata->start, node->pdata->size);
    }
    else if (node != NULL && node->pdata->is_occupied == 0)
    {
        printf("Status: FREE Start index: %d Length: %ld\n",
               node->pdata->start, node->pdata->size);
    }
    if (node->next == NULL)
    {
        printf("Status: FREE Start index: %ld Length: %d\n",
               node->pdata->start + node->pdata->size, mem_left);
    }
}

void *find_available_memory(TNode *llist, size_t size)
{
    reset_traverser(llist, FRONT);
    TNode *node;
    void *key = NULL;
    TNode *bestNode = NULL;
    int minSizeDifference = MEMSIZE;
    do
    {
        node = succ(llist);
        if (node != NULL && node->pdata->is_occupied == 0 && node->pdata->size >= size)
        {
            int difference = node->pdata->size - size;
            if (difference < minSizeDifference)
            {
                minSizeDifference = difference;
                bestNode = node;
            }
            else if (difference == 0)
            {
                key = (char *)_heap + node->pdata->start;
                return key;
            }
        }
    } while (node != NULL);
    if (bestNode != NULL)
    {
        key = (char *)_heap + bestNode->pdata->start;
    }
    return key;
}

void print_memlist()
{
    if (_memlist == NULL)
        printf("Status: FREE Start index: 0 Length: %d\n", mem_left);
    process_list(_memlist, print_node);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size)
{
    TData *data;
    TNode *node;
    data = (TData *)malloc(sizeof(TData));
    void *key;
    if (_memlist == NULL)
    {
        data->start = 0;
        key = (char *)_heap + data->start;
        mem_left = mem_left - size;
    }
    else
    {
        key = find_available_memory(_memlist, size);

        if (key == NULL)
        {
            if (mem_left >= size)
            {
                TNode *tail = _memlist->tail;
                key = (char *)_heap + tail->key + tail->pdata->size;
                data->start = tail->pdata->size + tail->pdata->start;
                mem_left = mem_left - size;
            }
            else
                return NULL;
        }
        else
        {
            data->start = get_index(key);
        }
    }
    data->is_occupied = 1;
    data->size = size;
    data->ptr = key;
    node = make_node(data->start, data);
    TNode *found_node = find_node(_memlist, data->start);
    if (found_node != NULL) // if there already exist a node that has the same start value
    {
        found_node->key = found_node->key + size;
        if (found_node->pdata->size - size == 0)
        {
            delete_node(&_memlist, found_node);
        }
        else
        {
            found_node->pdata->start = found_node->pdata->start + size;
            found_node->pdata->size = found_node->pdata->size - size;
        }
    }
    insert_node(&_memlist, node, ASCENDING);
    return &(_heap[get_index(key)]);
}

// Frees memory pointer to by ptr.
void myfree(void *ptr)
{
    TNode *node = find_node(_memlist, (int)get_index(ptr));
    free(ptr);
    if (node == NULL)
    {
        // printf("[ERROR] unable to find node to free.\n");
    }
    else
    {
        node->pdata->is_occupied = 0;
        TNode *temp = NULL;
        if (node->prev != NULL && node->prev->pdata->is_occupied == 0)
        {
            temp = node->prev;
            merge_node(_memlist, node, 0);
        }
        if (node->next != NULL && node->next->pdata->is_occupied == 0)
        {
            if (temp != NULL)
                node = temp;
            merge_node(_memlist, node, 1);
        }
        // Only node in the list
        if (node->next == NULL && _memlist->tail == temp)
        {
            mem_left = mem_left + temp->pdata->size;
            delete_node(&_memlist, temp);
        }
    }
}
