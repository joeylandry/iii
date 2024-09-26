/**************************************************************
 *
 *                     bit2.h
 *
 *     Assignment: iii
 *     Authors:    Joey Landry & Arshiya Lall 
 *     Date:       September 18, 2024
 *
 *     summary
 *
 *     ...you may provide more information here about 
 *        the program or file, it's interfaces, etc. here
 *
 **************************************************************/
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include <bit.h>

/* Include Guard -- Protects against double #define (infinite loop) */
#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

/* Define UArray2_T as the main type, T as short hand */
typedef struct Bit2_T *Bit2_T;

struct Bit2_T {
    int width; //number of columns
    int height; //number of rows 
    Bit_T data; 
};

/* TODO: 
 * !! CHECK POINTERS !!
 */

/********** Bit2_new ********
 *
 * Creates a new 2D array of bits 
 *
 * Parameters:
 *      int width: number of columns in the array
 *      int height: number of rows in the array
 *
 * Return: A new Bit2_T object
 *
 * Expects
 *      width and height must be greater than 0
 * 
 * Notes:
 *      Checked runtime error if width/height less than 0
 *      Allocates memory using Hanson UArray_T
 ************************/
Bit2_T Bit2_new(int width, int height); /*SIZE??*/
//check if width and hieght are valid greater than 0
//allocate memory for bit2_t struct
//set width and hieght
//calculate how many bits need to be stored: width * height
//Use hanson array to allocated memory of bits, round up to ensure space
// return Bit2_t

/********** Bit2_free ********
 *
 * Frees the memory allocated for the Bit2 array
 *
 * Parameters:
 *      Bit2_T *bit2: a pointer to the Bit2 array to be freed
 *
 * Return: None
 *
 * Expects
 *      'bit2' must not be NULL
 * 
 * Notes:
 *      Checked runtime error if 'bit2' is NULL
 ************************/
void Bit2_free(Bit2_T *bit2);
//ensure bit2 is not null
// free memoery allocated for underlying UArray_T using UArray_free
//Free Bit2_T struct itself
 
/********** Bit2_width ********
 *
 * Returns the width (num columns) of the Bit2 array
 *
 * Parameters:
 *      Bit2_T bit2: the Bit2 array
 *
 * Return: The width (num columns) of the array
 *
 * Expects
 *      'bit2' must not be NULL
 * 
 * Notes:
 *      Checked runtime error if 'bit2' is NULL
 ************************/
int Bit2_width(Bit2_T bit2);
//ensure bit2 is not null
// return width from Bit2_t struct
 
/********** Bit2_height ********
 *
 * Returns height (num rows) of the Bit2 array
 *
 * Parameters:
 *      Bit2_T bit2: the Bit2 array
 *
 * Return: The height (num rows) of the array
 *
 * Expects
 *      'bit2' must not be NULL
 * 
 * Notes:
 *      Checked runtime error if 'bit2' is NULL
 ************************/
int Bit2_height(Bit2_T bit2);
//ensure bit2 is not null
// return height from Bit2_t struct
 

/********** Bit2_put ********
 *
 * Sets the value or the bit at (row, col) to the given value (0 or 1)
 *
 * Parameters:
 *      Bit2_T bit2: the Bit2 array
 *      int row:     the row index 
 *      int col:     the column index
 *      int value:   the value to set (0 or 1)
 *
 * Return: The previous value at (row, col)
 *
 * Expects
 *      'bit2' must not be NULL
 *      row and col must be in bounds of array
 *      value must be 0 or 1
 * 
 * Notes:
 *      Checked runtime error if 'bit2' is NULL
 *      Checked runtime error if row/col out of bounds
 *      Checked runtime error if value not 0 or 1
 ************************/
int Bit2_put(Bit2_T bit2, int col, int row, int value);
// ensure bit2, row, col are within bounds
// compute 1d bit index: row *width + col
// calculate position of bit in the approporate integer (int_index = index/32)
//calculate which bit wihtin the integer to set (bit_index = index % 32)
//set or clear the bit by modifying the apporpirate bit in the unsigned int

/********** Bit2_get ********
 *
 * Returns the value of the bit at (row, col) as 0 or 1
 *
 * Parameters:
 *      Bit2_T bit2: the Bit2 array
 *      int row:     the row index 
 *      int col:     the column index
 *
 * Return: The value (0 or 1) of the bit at (row, col)
 *
 * Expects
 *      'bit2' must not be NULL
 *      row and col must be in bounds of array
 * 
 * Notes:
 *      Checked runtime error if 'bit2' is NULL
 *      Checked runtime error if row/col out of bounds
 ************************/
int Bit2_get(Bit2_T bit2, int col, int row);
// ensure bit2, row, col are in bounds
// compute 1d bit index using row*width+col
//use appropriate int_index and bit_index to retrieve value of the bit

/********** Bit2_map_row_major ********
 *
 * Calls apply function for each element in the bit array in row-major order
 *
 * Parameters:
 *      Bit2_T bit2: the Bit2 array
 *      void apply(int row, int col, Bit2_T bit2, int bit_elem, void *cl):
 *                   the function to apply to each element
 *      void *cl:    closure parameter given to apply function
 *
 * Return: None
 *
 * Expects
 *      'bit2' and apply must not be NULL
 * 
 * Notes:
 *      Checked runtime error if 'bit2' or apply is NULL
 *      Calls apply for each element in row-major order
 *      Column indices vary more rapidly
 ************************/
/* What is int bit_elem for? Is this right? */
void Bit2_map_row_major(Bit2_T bit2, void apply(int col, int row, Bit2_T bit2, int bit_elem, void *cl), void *cl);
//traverse the 2d bit array in row major order
// for each element, call the apply function with the (row, col) and bit value

/********** Bit2_map_col_major ********
 *
 * Calls apply function for each element in the bit array in column-major order
 *
 * Parameters:
 *      Bit2_T bit2: the Bit2 array
 *      void apply(int row, int col, Bit2_T bit2, int bit_elem, void *cl):
 *                   the function to apply to each element
 *      void *cl:    closure parameter given to apply function
 *
 * Return: None
 *
 * Expects
 *      'bit2' and apply must not be NULL
 * 
 * Notes:
 *      Checked runtime error if 'bit2' or apply is NULL
 *      Calls apply for each element in col-major order
 *      Row indices vary more rapidly
 ************************/
/* What is int bit_elem for? Is this right? */
void Bit2_map_col_major(Bit2_T bit2, void apply(int col, int row, Bit2_T bit2, int bit_elem, void *cl), void *cl);
//traverse 2d array in column major order
// for each element call the apply function with (row, col) and the bit value

#endif 
