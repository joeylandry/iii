#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <pnmrdr.h>
#include <uarray2.h>

#define MAX_SIZE 9
#define SUBGRID 3

typedef struct {
    int current_line;  
    int row_col_check;  // 1 for row check, 0 for col check
    int count[9];       // Array to count how many times each number (1-9) has been seen
} Seen;


Pnmrdr_T check_pgm(FILE *file) 
{
    Pnmrdr_T reader = Pnmrdr_new(file);
    Pnmrdr_mapdata data = Pnmrdr_data(reader);

    if (data.type != Pnmrdr_gray) {
        fprintf(stderr, "Error: Must input a PGM file.\n");
        Pnmrdr_free(&reader);
        exit(EXIT_FAILURE);
    }

    if (data.width != 9 || data.height != 9 || data.denominator != 9) {
        fprintf(stderr, "Error: Incorrect dimensions.\n");
        Pnmrdr_free(&reader);
        exit(EXIT_FAILURE);
    }

    return reader;
}

UArray2_T populate_sudoku(Pnmrdr_T reader) 
{
    UArray2_T sudoku = UArray2_new(MAX_SIZE, MAX_SIZE, sizeof(int));

    for (int row = 0; row < MAX_SIZE; row++) {
        for (int col = 0; col < MAX_SIZE; col++) {
            int element = Pnmrdr_get(reader);

            if (element < 1 || element > 9) {
                fprintf(stderr, "Error: Invalid pixel value (%u) in Sudoku PGM file.\n", element);
                UArray2_free(&sudoku);
                Pnmrdr_free(&reader);
                exit(EXIT_FAILURE);
            }

            int *cell = UArray2_at(sudoku, col, row);
            *cell = element;
        } 
    }

    Pnmrdr_free(&reader);

    return sudoku;
}

void apply(int col, int row, UArray2_T sudoku, void *elem, void *cl) 
{
    (void)sudoku;
    Seen *seen = (Seen *)cl;    // Closure contains current row, column, or subgrid, and count array
    int *value = (int *)elem;

    if (seen->row_col_check == 2) {  // Subgrid check
        // Calculate the top-left corner of the subgrid that the current element belongs to
        int subgrid_start_row = (row / SUBGRID) * SUBGRID;
        int subgrid_start_col = (col / SUBGRID) * SUBGRID;

        // We are now entering a new subgrid; reset the count array for this subgrid
        for (int i = 0; i < 9; i++) {
            seen->count[i] = 0;  // Reset count array for the current subgrid
        }

        // Traverse the current 3x3 subgrid
        for (int sub_row = subgrid_start_row; sub_row < subgrid_start_row + SUBGRID; sub_row++) {
            for (int sub_col = subgrid_start_col; sub_col < subgrid_start_col + SUBGRID; sub_col++) {
                int *subgrid_value = UArray2_at(sudoku, sub_col, sub_row);
                int subgrid_index = *subgrid_value - 1;
                seen->count[subgrid_index]++;

                // If a number has been seen more than once in the subgrid, report an error
                if (seen->count[subgrid_index] > 1) {
                    fprintf(stderr, "ERROR: Repeated number %d found in subgrid starting at (%d, %d)\n", 
                            *subgrid_value, subgrid_start_row, subgrid_start_col);
                    exit(EXIT_FAILURE);
                }
            }
        }
        return;  // Subgrid check is complete, return from apply
    }

    int current_line;
    // For row and column checks
    if (seen->row_col_check == 1) {  // Row check
        current_line = row;
    } else {  // Column check
        current_line = col;
    }

    // Reset the count array when moving to a new row or column
    if (current_line != seen->current_line) {
        seen->current_line = current_line;
        for (int i = 0; i < 9; i++) {
            seen->count[i] = 0;  // Reset count array for each new row or column
        }
    }

    int index = *value - 1;  // Convert value to 0-based index (value 1 -> index 0)
    seen->count[index]++;

    // Check if the number has appeared more than once
    if (seen->count[index] > 1) {
        if (seen->row_col_check == 1) {  // Row repeat error
            fprintf(stderr, "ERROR: Repeated number %d found in row %d at position (%d, %d)\n", 
                    *value, row, col, row);
            exit(EXIT_FAILURE);
        } else {  // Column repeat error
            fprintf(stderr, "ERROR: Repeated number %d found in column %d at position (%d, %d)\n", 
                    *value, col, col, row);
            exit(EXIT_FAILURE);
        }
    }
    // (void)sudoku;
    // Seen *seen = (Seen *)cl;    // Closure contains current row and count array
    // int *value = (int *)elem;

    // int current_line;
    // int subgrid_index;

    // if (seen->row_col_check == 2) {
    //     int sub_row = row / 3;
    //     int sub_col = col / 3;
    //     subgrid_index = sub_row * 3 + sub_col;
    //     current_line = subgrid_index;
    // } else if (seen->row_col_check == 1) { /* Row Check */
    //     current_line = row;
    // } else {
    //     current_line = col; /* Col Check */
    // }

    // if (current_line != seen->current_line) {
    //     seen->current_line = current_line;
    //     for (int i = 0; i < 9; i++) {
    //         seen->count[i] = 0;  // Reset counter array for each new row or column
    //     }
    // }

    // int index = *value - 1;      // Convert value to 0-based index (value 1 -> index 0)
    // seen->count[index]++;

    // // Check if the number has appeared more than once
    // if (seen->count[index] > 1) {
    //     if (seen->row_col_check == 2) {
    //         int sub_row = (row / 3) * 3;
    //         int sub_col = (col / 3) * 3;
    //         fprintf(stderr, "ERROR: Repeated number %d found in subgrid starting at (%d, %d)\n", *value, sub_row, sub_col);
    //         exit(EXIT_FAILURE);
    //     }
    //     if (seen->row_col_check) {
    //         fprintf(stderr, "ERROR: Repeated number %d found in row %d at position (%d, %d)\n", *value, row, col, row);
    //         exit(EXIT_FAILURE);
    //     } else {
    //         fprintf(stderr, "ERROR: Repeated number %d found in column %d at position (%d, %d)\n", *value, col, col, row);
    //         exit(EXIT_FAILURE);
    //     }
    // }

}

void check_sudoku(UArray2_T sudoku) 
{
    Seen seen;
    seen.current_line = -1;    // Initialize to an invalid row/col index
    seen.row_col_check = 1;     // Indicate that we are checking rows

    UArray2_map_row_major(sudoku, apply, &seen);

    seen.current_line = -1;    // Initialize to an invalid row/col index
    seen.row_col_check = 0; //indicate checking col

    UArray2_map_col_major(sudoku, apply, &seen);

    seen.current_line = -1;    // Initialize to an invalid row/col index
    seen.row_col_check = 2;     // Indicate that we are checking rows

    UArray2_map_row_major(sudoku, apply, &seen);

    // If no duplicates are found
    printf("Sudoku rows and columns are valid.\n");
}

int main(int argc, char *argv[]) 
{
    /* Read from stdin if 1 argument is supplied */
    FILE *file = stdin;
    /* Open the file if it is provided */
    if (argc == 2) {
        file = fopen(argv[1], "r");
        assert(file != NULL);
    }

    /* Create a UArray2_T to store the sudoko grid */
    Pnmrdr_T reader = check_pgm(file);

    UArray2_T sudoku = populate_sudoku(reader);

    check_sudoku(sudoku);

    // /* Print the Sudoku grid */
    // for (int row = 0; row < MAX_SIZE; row++) {
    //     for (int col = 0; col < MAX_SIZE; col++) {
    //         int *cell = UArray2_at(sudoku, col, row);
    //         printf("%d ", *cell);  // Print each cell
    //     }
    //     printf("\n");  // Newline after each row
    // }
    
    /* Clean up and close file */
    if (argc == 2) {
        fclose(file);
    }

    UArray2_free(&sudoku);
    return 0;
}