#include <stdio.h>
#include "bitmap.h"

// IMPLEMENTED FOR YOU
// Utility function to print out an array of char as bits
// Print the entire bitmap. Arguments: The bitmap itself, and the length of the bitmap
// Each bit of the bitmap represents 1 byte of memory. 0 = free, 1 = allocated.
// map: The bitmap itself, an array of unsigned char.
//      Each bit of the bitmap represents one byte of memory
// len: The length of the bitmap array in characters
//
// Returns: Nothing
void print_map(unsigned char *map, int len)
{
    int i, j;

    for (i = 0; i < len; i++)
    {

        unsigned char mask = 0b10000000;
        for (j = 0; j < 8; j++)
        {
            if (map[i] & mask)
                printf("1");
            else
                printf("0");
            mask = mask >> 1;
        }
        printf(" ");
    }
    printf("\n");
}

// Search the bitmap for the required number of zeroes (representing
// free bytes of memory). Returns index of first stretch of 0s
// that meet the criteria. You can use this as an index into
// an array of char that represents the process heap
// bitmap = Bitmap declared as an array of unsigned char
// len = Length of bitmap in characters
// num_zeroes = Length of string of 0's required
// Returns: Index to stretch of 0's of required length, -1 if no such stretch can be found

long search_map(unsigned char *bitmap, int len, long num_zeroes)
{
    int i, j, skip_index = 0, return_index = 0, zeros_count = 0;
    unsigned char first_bit = 0b10000000;

    // printf("=============== CASE START ================\n");
    for (i = 0; i < len; i++)
    {
        unsigned char mask = 0b10000000;
        unsigned char next_mask = 0b01000000;
        for (j = 0; j < 8; j++)
        {
            // printf("bitmap[%d] index: %d\n", i, j);
            // Found bit 1
            if (bitmap[i] & mask)
            {
                return_index = return_index + 1;
                // printf("[STATE] increment of return_index value: %d\n", return_index);
                // Reset consec. zero count
                zeros_count = 0;
                // printf("[RESET]\n");
            }
            // Found bit 0
            else
            {
                zeros_count = zeros_count + 1;
                // printf("[STATE] return_index: %d, zero count: %d\n", return_index, zeros_count);
                if (zeros_count == num_zeroes)
                {

                    // printf("[RETURN] with INDEX value: %d\n", return_index);
                    // printf("=============== CASE END ================\n");
                    return return_index;
                }
                else
                {
                    skip_index = skip_index + 1;
                    // printf("[STATE] skip_index: %d\n", skip_index);
                    if (bitmap[i] & next_mask || (j == 7) && ((i + 1) < len && bitmap[i + 1] & first_bit))
                    {
                        return_index = return_index + skip_index;
                        skip_index = 0;
                        // printf("[STATE] increment of return_index value: %d\n", return_index);
                    }
                }
            }
            mask = mask >> 1;
            next_mask = next_mask >> 1;
        }
    }

    return -1;
} // main

void print_bytes(unsigned char map)
{
    unsigned char k = 0b10000000;
    for (int j = 0; j < 8; j++)
    {
        if (map & k)
            printf("1");
        else
            printf("0");
        k = k >> 1;
    }
    printf("\n");
}

// Set map bits to 0 or 1 depending on whether value is non-zero
// map = Bitmap, declared as an array of unsigned char
// start = Starting index to mark as 1 or 0
// length = Number of bits to mark
// value = Value to mark the bits as. value = 0 marks the bits
//          as 0, non-zero marks the bits as 1
// Returns: Nothing

void set_map(unsigned char *map, long start, long length, int value)
{

    /**
     * @brief
     * Determine the mask base on start and length (offset)
     * length - no. of 1s (or 0s)
     * start - no. of byte shift towards right ( >> )
     * e.g. unsigned char mask = 0b00000000
     * length = 2, start = 2, value = 1;
     * then, 0b10000000 (since value = 1)
     *  1. create mask that is 0b11000000 (length = 2).
     *  2. slr (>>) mask by 2 (start = 2).
     * determine the position and range to perform bitwise operations
     *  1. given the start, find the index to access map
     *      1.a  eqn: int n = start / 8
     *      1.b  total_char_length = (n * 8) - 1 (8,16,24,32 etc)
     *  2. create counter to track the no. of bytes change
     *      2.a  if total_char_length - n < start:
     *          2.a.1  continue process of masking the NEXT index of bytes
     *          2.a.2  done
     */

    int ndx = start / 8;

    int is_done = 0;
    int is_next_iter = 0;
    int iter = length - 1;

    while (is_done == 0)
    {
        int char_limit_at_index = (ndx == 0) ? 8 : ((ndx + 1) * 8);
        int bytes_left = char_limit_at_index - start;
        unsigned char mask = 0b10000000;
        unsigned char temp_mask = 0b00000000;
        iter = (length < bytes_left) ? length - 1 : bytes_left - 1;
        // printf("[STATE] length: %ld\n", length);
        // printf("[STATE] bytes_left: %d at index: %d\n", bytes_left, ndx);
        // printf("\n[STATE] iter: %d\n", iter);
        while (iter > 0)
        {
            temp_mask = mask | temp_mask;
            mask = mask >> 1;
            mask = mask | temp_mask;
            iter = iter - 1;
        }
        if (is_next_iter == 0)
        {
            mask = mask >> 8 - bytes_left;
        }

        // printf("[STATE] ndx: %d\n", ndx);
        // printf("[STATE] start: %ld\n", start);
        // printf("[STATE] char_limit_at_index: %d\n", char_limit_at_index);
        // printf("\n[STATE] mask: ");
        // print_bytes(mask);
        // printf("\n");

        if (value == 1)
        {
            map[ndx] = map[ndx] | mask;
        }
        else
        {
            mask = ~mask;
            map[ndx] = map[ndx] & mask;
        }
        // since length does not spill over to another index in the bitmap
        if (length <= bytes_left)
        {
            is_done = 1;
        }
        // length spill over to another index in the bitmap
        // continue process with another mask (do not require to shift bit)
        else
        {
            is_next_iter = 1;
            ndx = ndx + 1;
            start = start + bytes_left;
            length = length - bytes_left;
        }
    }
}

// IMPLEMENTED FOR YOU
// Marks a stretch of bits as "1", representing allocated memory
// map = Bitmap declared as array of unsigned char
// start = Starting index to mark
// length = Number of bits to mark as "1"
void allocate_map(unsigned char *map, long start, long length)
{
    set_map(map, start, length, 1);
}

// IMPLEMENTED FOR YOU
// Marks a stretch of bits as "0", representing allocated memory
// map = Bitmap declared as array of unsigned char
// start = Starting index to mark
// length = Number of bits to mark as "0"
void free_map(unsigned char *map, long start, long length)
{
    set_map(map, start, length, 0);
}
