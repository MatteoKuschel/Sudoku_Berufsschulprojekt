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
            printf("%i ",sudoku_grid[i][m]);
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

double update_time(clock_t start_clock)
{
    clock_t current_clock;
    double current_time;

    current_clock = clock();

    current_time = (current_clock - start_clock)/CLOCKS_PER_SEC;

    return current_time;
}

void get_current_time(double current_time, int *hours, int *minutes, int *seconds)
{
    *hours = current_time/3600;
    current_time-= *hours*3600;

    *minutes = current_time/60;
    current_time -= *minutes*60;

    *seconds = current_time;
}
