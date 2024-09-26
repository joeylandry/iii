
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "bit2.h"
#include <pnmrdr.h>
#include "except.h"
#include "seq.h"

typedef struct {
    int col;
    int row;
} Coord;

Bit2_T check_pbm(FILE *file) 
{
    Pnmrdr_T reader = Pnmrdr_new(file);
    Pnmrdr_mapdata data = Pnmrdr_data(reader);

    if (data.type != Pnmrdr_bit) {
        fprintf(stderr, "Error: Must input a PBM file.\n");
        Pnmrdr_free(&reader);
        exit(EXIT_FAILURE);
    }

    if (data.width <= 0 || data.height <= 0) {
        fprintf(stderr, "Error: Incorrect dimensions.\n");
        Pnmrdr_free(&reader);
        exit(EXIT_FAILURE);
    } 

    Bit2_T array = Bit2_new(data.width, data.height);

    /* Check to see if PBM data is supplied PROPERLY OR LESS THAN CORRECT AMOUNT */
    int pixels = data.width * data.height;
    for (int i = 0; i < pixels; i++) {
        int element = Pnmrdr_get(reader);
        /* Formula to deliver row, col indices */
        int row = i / data.width;
        int col = i % data.width;
        Bit2_put(array, col, row, element);

        if (element > 1) {  // PBM values are only 0 or 1
            fprintf(stderr, "Error: Invalid PBM data. Pixels should be 0 or 1.\n");
            Pnmrdr_free(&reader);
            exit(EXIT_FAILURE);
        }
    }

    Pnmrdr_free(&reader);

    return array;
}

void mark_perimeter(int col, int row, Bit2_T bit2, int bit_elem, void *cl)
{
    /* If black pixel */
    if (bit_elem == 1) {
        int width = Bit2_width(bit2);
        int height = Bit2_height(bit2);

        /* Check to see if pixel is on the edge */
        if (col == 0 || row == 0 || col == width - 1 || row == height - 1) {
            Seq_T queue = (Seq_T)cl; /* Retrieve the queue */
            Coord *coord = malloc(sizeof(Coord)); 
            coord->col = col;
            coord->row = row;
            Seq_addhi(queue, coord); /* Add the coordinate to the queue */
        }

    }
}

void bfs_search(Bit2_T bit2, Bit2_T visited, Seq_T queue)
{
    int width = Bit2_width(bit2);
    int height = Bit2_height(bit2);

    /* Maintain a loop until the queue is empty */
    while (Seq_length(queue) > 0) {
        /* Dequeue the first/next pixel for processing */ 
        Coord *curr = Seq_remlo(queue);
        int col = curr->col; /* Col index of curr pixel */
        int row = curr->row; /* Row index of curr pixel */
        free(curr); /* No longer need curr, coords are saved */

        /* Only process the pixel if it hasn't been visited */
        if (Bit2_get(visited, col, row) == 0) {
            /* Mark pixel 1 (visited) in visit array */
            Bit2_put(visited, col, row, 1); 

            /* Check neighbors and enqueue if black pixel */

            /* Check the left neighbor to see if black and unvisited */
            if (col > 0 && Bit2_get(bit2, col - 1, row) == 1 && Bit2_get(visited, col - 1, row) == 0) {
                Coord *left = malloc(sizeof(Coord));
                left->col = col - 1; /* Col index of left neighbor */
                left->row = row; /* Row index of left neighbor */
                Seq_addhi(queue, left); /* Enqueue left neigbor for checking */
            }

            /* Check the right neighbor to see if black and unvisited */
            if (col < width - 1 && Bit2_get(bit2, col + 1, row) == 1 && Bit2_get(visited, col + 1, row) == 0) {
                Coord *right = malloc(sizeof(Coord));
                right->col = col + 1; /* Col index of right neighbor */
                right->row = row; /* Row index of right neighbor */
                Seq_addhi(queue, right); /* Enqueue right neigbor for checking */
            }

            /* Check the above nieghbor to see if black and unvisited */
            if (row > 0 && Bit2_get(bit2, col, row - 1) == 1 && Bit2_get(visited, col, row - 1) == 0) {
                Coord *top = malloc(sizeof(Coord));
                top->col = col;  /* Col index of top neighbor */
                top->row = row - 1;  /* Row index of top neighbor */
                Seq_addhi(queue, top);   /* Enqueue top neigbor for checking */
            }

            /* Check the bottom nieghbor to see if black and unvisited */
            if (row < height - 1 && Bit2_get(bit2, col, row + 1) == 1 && Bit2_get(visited, col, row + 1) == 0) {
                Coord *bot = malloc(sizeof(Coord));
                bot->col = col;  /* Col index of bottom neighbor */
                bot->row = row + 1;  /* Row index of bottom neighbor */
                Seq_addhi(queue, bot);   /* Enqueue bottom neigbor for checking */
            }
        }
    }
}

void convert_pixels(int col, int row, Bit2_T bit2, int bit_elem, void *cl)
{
    (void)bit_elem;
    if (Bit2_get((Bit2_T)cl, col, row) == 1) {  /* If visited (black edge-connected) */
        Bit2_put(bit2, col, row, 0);  /* Set to white */
    }
}

void remove_black_edges(Bit2_T bit_arr) 
{
    int width = Bit2_width(bit_arr);
    int height = Bit2_height(bit_arr);

    /* Array to track visited black pixels */
    Bit2_T visited = Bit2_new(width, height);

    /* Create a queue using Hanson seq to store coords of black edges */
    Seq_T queue = Seq_new(0);

    /* Identify black edge pixels on perimeter - enqueue for BFS */
    Bit2_map_row_major(bit_arr, mark_perimeter, queue);

    /* BFS to locate black edge-connected pixels */
    bfs_search(bit_arr, visited, queue);

    /* Convert all black edge pixels to white */
    Bit2_map_row_major(bit_arr, convert_pixels, visited);

    Bit2_free(&visited);
    Seq_free(&queue);
}

void apply_write(int col, int row, Bit2_T bit2, int bit_elem, void *cl) 
{
    (void)row;
    FILE *outputfp = (FILE *)cl;
    fprintf(outputfp, "%d ", bit_elem);
    if (col == Bit2_width(bit2) - 1) {
        fprintf(outputfp, "\n");
    }
}


void write(FILE *outputfp, Bit2_T bit_arr) 
{
    int width = Bit2_width(bit_arr);
    int height = Bit2_height(bit_arr);
    
    fprintf(outputfp, "P1\n");
    fprintf(outputfp, "%d %d\n", width, height);
    
    Bit2_map_row_major(bit_arr, apply_write, outputfp);
}

int main(int argc, char *argv[]) 
{
    /* Read from stdin if 1 argument is supplied */
    FILE *file = stdin;
    /* Open the file if it is provided */
    if (argc == 2) {
        file = fopen(argv[1], "r");
        assert(file != NULL);
    } else {
        fprintf(stderr, "Too many arguments.\n");
        exit(EXIT_FAILURE);
    }

    /* Create a UArray2_T to store the sudoku grid */
    Bit2_T bit_arr = check_pbm(file);

    remove_black_edges(bit_arr);

    write(stdout, bit_arr);

    /* Clean up and close file */
    if (argc == 2) {
        fclose(file);
    }

    Bit2_free(&bit_arr);
    return 0;
}
