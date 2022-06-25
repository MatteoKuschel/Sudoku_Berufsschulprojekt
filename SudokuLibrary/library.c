#include "library.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int calculate_box(int row, int col)
{
    return ((int) row / 3) * 3 + ((int) col / 3);
}

int check_on_duplication(int number, int row, int col)
{
    if (sudoku_boxes[calculate_box(row, col)]->numbers[number-1] == number) {
        return 1;
    }

    if (sudoku_cols[col]->numbers[number-1] == number) {
        return 1;
    }

    if (sudoku_rows[row]->numbers[number-1] == number) {
        return 1;
    }

    return 0;
}

void insert_number(int number, int row, int col)
{
    int box_index = calculate_box(row, col);
    sudoku_boxes[box_index]->numbers[number-1] = number;
    sudoku_boxes[box_index]->free_numbers = sudoku_boxes[box_index]->free_numbers--;

    sudoku_rows[row]->numbers[number-1] = number;
    sudoku_rows[row]->free_numbers = sudoku_rows[row]->free_numbers--;

    sudoku_cols[col]->numbers[number-1] = number;
    sudoku_cols[col]->free_numbers = sudoku_cols[col]->free_numbers--;
}

void delete_number(int number, int row, int col)
{
    int box_index = calculate_box(row, col);
    sudoku_boxes[box_index]->numbers[number-1] = 0;
    sudoku_boxes[box_index]->free_numbers = sudoku_boxes[box_index]->free_numbers++;

    sudoku_rows[row]->numbers[number-1] = 0;
    sudoku_rows[row]->free_numbers = sudoku_rows[row]->free_numbers++;

    sudoku_cols[col]->numbers[number-1] = 0;
    sudoku_cols[col]->free_numbers = sudoku_cols[col]->free_numbers++;
}

void shuffle(int *array)
{
    srand(time(0));
    for (int i = 0; i < 9 - 1; i++) {
        int j = i + rand() / (RAND_MAX / (9 - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

int fill_grid()
{
    int row = 0;
    int col = 0;

    for (int i = 0; i < 81; i++) {
        row = ((int) i / 9);
        col = i % 9;

        if (sudoku_grid[row][col] == 0) {
            int array[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            shuffle(array);

            for (int j = 0; j < 9; j++) {
                if (!check_on_duplication(array[j], row, col)) {
                    sudoku_grid[row][col] = array[j];
                    insert_number(array[j], row, col);

                    if (fill_grid() && sudoku_grid[8][8] != 0) {
                        return 1;
                    }

                }
                delete_number(sudoku_grid[row][col], row, col);
            }
            break;
        }
    }

    if (sudoku_grid[8][8] != 0) {
        return 1;
    }

    sudoku_grid[row][col] = 0;

    return 0;
}

void print_sudoku()
{
    printf("GRID\n");
    for (int i = 0; i < 9; i++) {
        for (int m = 0; m < 9; m++) {
            printf("%i ",sudoku_grid_copy[i][m]);
        }
        printf("\n");
    }
}

void initialize_structs()
{
    for (int i = 0; i < 9; ++i) {
        sudoku_boxes[i] = malloc(sizeof(Sudoku_box));
        sudoku_cols[i] = malloc(sizeof(Sudoku_column));
        sudoku_rows[i] = malloc(sizeof(Sudoku_row));

        sudoku_boxes[i]->free_numbers = 9;
        sudoku_cols[i]->free_numbers = 9;
        sudoku_rows[i]->free_numbers = 9;

        for (int j = 0; j < 9; ++j) {
            sudoku_boxes[i]->numbers[j] = 0;
            sudoku_cols[i]->numbers[j] = 0;
            sudoku_rows[i]->numbers[j] = 0;
        }
    }
}

void set_difficulty(int chosen_difficulty)
{
    switch (chosen_difficulty) {
        case 1:
            digits_to_remove = 24;
            break;
        case 2:
            digits_to_remove = 35;
            break;
        case 3:
            digits_to_remove = 50;
            break;
        default:
            digits_to_remove = 24;
    }
}

void copy_sudoku_grid()
{
    for (int i = 0; i < 9 ; i++) {
        for (int j = 0; j < 9 ; j++) {
            sudoku_grid_copy[i][j] = sudoku_grid[i][j];
        }
    }
}

void remove_sudoku_grid_numbers()
{
    int remaining_digits = digits_to_remove;
    srand(time(NULL));
    do {
        int cell = rand() % 81;
        int row = ((int) cell / 9);
        int col = cell % 9;
        if (sudoku_grid_copy[row][col] != 0) {
            delete_number(sudoku_grid_copy[row][col], row, col);
            sudoku_grid_copy[row][col] = 0;
            remaining_digits--;
        }
    } while (remaining_digits > 0);
}
