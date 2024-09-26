/*
 *                      useuarray2.c
 *
 *         This program illustrates the use of the uarray2 interface.
 *
 *         Although it will catch some errors in some uarray2 implementations
 *         it is NOT a thorough test program.
 *
 *         NOTE: this program is commented sparsely, as figuring out
 *         what this program does and why the tests it makes matter is
 *         part of the homework assignment.
 *
 *         Author: Noah Mendelsohn
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uarray2.h"

 typedef long number;

 const int DIM1 = 5;
 const int DIM2 = 7;
 const int ELEMENT_SIZE = sizeof(number);
// const int MARKER = 99;

// void
// check_and_print(int i, int j, UArray2_T a, void *p1, void *p2) 
// {
//         number *entry_p = p1;

//         *((bool *)p2) &= UArray2_at(a, i, j) == entry_p;

//         if ( (i == (DIM1 - 1) ) && (j == (DIM2 - 1) ) ) {
//                 /* we got the corner */
//                 *((bool *)p2) &= (*entry_p == MARKER);
//         }

//         printf("ar[%d,%d]\n", i, j);
// }

void test_UArray2_new(UArray2_T test_array) {
        if (test_array == NULL) {
                printf("Failed to create UArray2.\n");
        }

        /* Confirm array is being properly initialized */
        printf("UArray2 created with width: %d, height: %d, element size: %d bytes.\n",
           test_array->width, test_array->height, test_array->elem_size);
}

void test_UArray2_structs(UArray2_T test_array) {
        printf("UArray2 created with dimensions %d x %d and element size %d bytes.\n",
           UArray2_width(test_array), UArray2_height(test_array), UArray2_size(test_array));
}

void test_UArray2_free(UArray2_T test_array) {
        //test_array == NULL;
        UArray2_free(&test_array);
        assert(test_array == NULL);
        if (test_array == NULL) {
                printf("Memory was successfully freed and the pointer was set to NULL.\n");
        } else {
                printf("Memory was not properly freed.\n");
    }
}

UArray2_T populate_array(UArray2_T pop_array) {
        for (int i = 0; i < UArray2_height(pop_array) * UArray2_width(pop_array); i++) {
                int *element = UArray_at(pop_array->data, i);
                *element = i;
        }
        return pop_array;
        
}

void print_array_2D(UArray2_T pop_array) {
        printf("Printing array in 2D format:\n");

        /* Iterate through the array row by row */
        for (int i = 0; i < UArray2_height(pop_array); i++) {
                for (int j = 0; j < UArray2_width(pop_array); j++) {
                        /* Calculate the linear index and access the element */
                        int index = i * UArray2_width(pop_array) + j;
                        int *element = UArray_at(pop_array->data, index);  /* Access element */
                        printf("%d ", *element); 
                }
                printf("\n");
        }
}

void test_UArray2_at(UArray2_T pop_array) {
        printf("Printing array in 2D format using UArray2_at():\n");
        for (int i = 0; i < UArray2_height(pop_array); i++) {
                for (int j = 0; j < UArray2_width(pop_array); j++) {
                        /* Access elements using UArray2_at() */
                        int *element = UArray2_at(pop_array, i, j);
                        printf("%d ", *element); 
                }
                printf("\n");
        }

}



int
main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UArray2_T test_array;
        //bool OK = true;

        test_array = UArray2_new(DIM1, DIM2, ELEMENT_SIZE);
        test_UArray2_structs(test_array);
        test_UArray2_new(test_array);

        UArray2_T pop_array;
        pop_array = populate_array(test_array);
        print_array_2D(pop_array);
        test_UArray2_at(pop_array);
        test_UArray2_free(pop_array);
        //UArray2_at(pop_array, 0, 1);
        

        // OK = (UArray2_width(test_array) == DIM1) &&
        //      (UArray2_height(test_array) == DIM2) &&
        //      (UArray2_size(test_array) == ELEMENT_SIZE);


        /* Note: we are only setting a value on the corner of the array */
        // *((number *)UArray2_at(test_array, DIM1 - 1, DIM2 - 1)) = MARKER;

        // printf("Trying column major\n");
        // UArray2_map_col_major(test_array, check_and_print, &OK);

        // printf("Trying row major\n");
        // UArray2_map_row_major(test_array, check_and_print, &OK);

        // UArray2_free(&test_array);

        // printf("The array is %sOK!\n", (OK ? "" : "NOT "));

}
