#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"

char _heap[MEMSIZE] = {0};
int _sizes[MEMSIZE] = {0};
// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr)
{
    if (ptr == NULL)
        return -1;
    else
        return (long)((char *)ptr - &_heap[0]);
}

void print_memlist()
{
    // Implement this to call print_map from bitmap.c
    print_map(_heap, MEMSIZE / 8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size)
{
    long ndx = search_map(_heap, MEMSIZE / 8, size);
    if (ndx == -1)
        return NULL;
    else
    {
        void *ptr;
        allocate_map(_heap, ndx, size);
        // Explanation from https://stackoverflow.com/questions/20016508/how-to-use-void-pointer-as-function-return-type-in-c
        ptr = (char *)_heap + ndx;
        _sizes[ndx] = size;
        return ptr;
    }
}

// Frees memory pointer to by ptr.
void myfree(void *ptr)
{
    long ndx = get_index(ptr);
    free_map(_heap, ndx, _sizes[ndx]);
}
