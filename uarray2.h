/**************************************************************
 *
 *                     UArray2.h
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
#include "uarray.h"
#include "uarrayrep.h"
#include "mem.h"

/* Include Guard -- Protects against double #define (infinite loop) */
#ifndef UARRAY_INCLUDED
#define UARRAY_INCLUDED

/* T is now a pointer to the UArray2_T defined by struct T */
#define T UArray2_T
typedef struct T *T;

struct T {
    int width; /* The width (num columns) of the array */
    int height; /* The height (num rows) of the array */
    int elem_size; /* Size of data stored in each element of the array */
    UArray_T data; /* Hanson UArray representing our 2D array */
};

/********** UArray2_new ********
 *
 * Allocates and returns a new 2D array UArray2 containing 'width' by 'height'
 * elements and each element is 'size' bytes. 
 *
 * Parameters:
 *      int width:  the number of columns in the array
 *      int height: the number of rows in the array
 *      int size:   the byte size of each element
 *
 * Return: A new Urray2_T object
 *
 * Expects
 *      width, height, and size must be greater than zero
 * 
 * Notes:
 *      - Checked runtime error if width, size, or height are invalid
 *      - Allocates memory using Hanson UArray
 ************************/
T UArray2_new(int width, int height, int size);

/********** UArray2_free ********
 *
 * Frees all memory that is allocated for UArray2
 *
 * Parameters:
 *      UArray2_T *uarray: a pointer to the array that will be freed
 *
 * Return: None
 *
 * Expects
 *      'uarray' cannot be NULL
 * 
 * Notes:
 *      - Checked runtime error if 'uarray' is NULL
 *      - Frees memory for UArray2_T struct and UArray memory
 ************************/
void UArray2_free(T *uarray);

/********** UArray2_width ********
 *
 * Returns the number of columns found in the 2D array
 *
 * Parameters:
 *      UArray2_T uarray: the array being checked for its width
 *
 * Return: Width (num columns) in the array
 *
 * Expects
 *      'uarray' cannot be NULL
 * 
 * Notes:
 *      - Checked runtime error if 'uarray' is NULL
 ************************/
int UArray2_width(T uarray);

/********** UArray2_Height ********
 *
 * Returns number of rows found in the 2D array
 *
 * Parameters:
 *      UArray2_T uarray: the array being checked for its height
 *
 * Return: Height (num rows) in the array
 *
 * Expects
 *       'uarray' cannot be NULL
 * 
 * Notes:
 *      - Checked runtime error if 'uarray' is NULL
 ************************/
int UArray2_height(T uarray);

/********** UArray2_size ********
 *
 * Returns the size in bytes of each element of the array
 *
 * Parameters:
 *      UArray2_T uarray: the array being checked for byte size of each elem
 *
 * Return: The byte size of each element
 *
 * Expects
 *      'uarray' cannot be NULL
 * 
 * Notes:
 *      - Checked runtime error if 'uarray' is NULL
 ************************/
int UArray2_size(T uarray);


/********** UArray2_at ********
 *
 * Returns a pointer to the element at the specified row and column
 *
 * Parameters:
 *      UArray2_T uarray: the 2D array
 *      int row:          the row index
 *      int col:          the column index 
 *
 * Return: A pointer to the element at (row, col)
 *
 * Expects
 *      'uarray' cannot be NULL
 *      row and col has to be within bounds of the array
 * 
 * Notes:
 *      - Checked runtime error if 'uarray' is NULL or row/col is out of bounds
 ************************/
void *UArray2_at(T uarray, int row, int col);

/********** UArray2_map_row_major ********
 *
 * Calls the apply function for each element in the array in row-major order
 *
 * Parameters:
 *      UArray2_T array: the 2D array
 *      void apply(int row, int col, UArray2_ uarray, void *elem, void *cl): 
 *                       the function to apply to each element
 *      void *cl:        closure parameter passed to apply
 *
 * Return: None
 *
 * Expects
 *      'uarray' and apply cannot be NULL
 * 
 * Notes:
 *      - Checked runtime error if 'uarray' or apply is NULL
 *      - Column indices vary more rapidly
 ************************/
void UArray2_map_row_major(T uarray, void apply(int row, int col, T uarray, void *elem, void *cl), void *cl);

/********** UArray2_map_col_major ********
 *
 * Calls the apply function for each element in the array in col-major order
 *
 * Parameters:
 *      UArray2_T array: the 2D array
 *      void apply(int row, int col, UArray2_ uarray, void *elem, void *cl): 
 *                       the function to apply to each element
 *      void *cl:        closure parameter passed to apply
 *
 * Return: None
 *
 * Expects
 *      'uarray' and apply cannot be NULL
 * 
 * Notes:
 *      - Checked runtime error if 'uarray' or apply is NULL
 *      - Row indices vary more rapidly
 ************************/
void UArray2_map_column_major(T uarray, void apply(int row, int col, T uarray, void *elem, void *cl), void *cl);

