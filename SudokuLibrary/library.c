#include "library.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

Cell_to_solve_node *head = NULL;

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
    sudoku_rows[row]->numbers[number-1] = number;
    sudoku_cols[col]->numbers[number-1] = number;
}

void delete_number(int number, int row, int col)
{
    int box_index = calculate_box(row, col);
    sudoku_boxes[box_index]->numbers[number-1] = 0;
    sudoku_rows[row]->numbers[number-1] = 0;
    sudoku_cols[col]->numbers[number-1] = 0;
}

void increase_free_numbers(int row, int col)
{
    int box_index = calculate_box(row, col);
    sudoku_boxes[box_index]->free_numbers++;
    sudoku_rows[row]->free_numbers++;
    sudoku_cols[col]->free_numbers++;
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


        sudoku_boxes[i]->free_numbers = 0;
        sudoku_cols[i]->free_numbers = 0;
        sudoku_rows[i]->free_numbers = 0;

        for (int j = 0; j < 9; ++j) {
            sudoku_boxes[i]->numbers[j] = 0;
            sudoku_cols[i]->numbers[j] = 0;
            sudoku_rows[i]->numbers[j] = 0;
        }
    }
}

void initialize_tables()
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cells_to_solve_box_table[i][j] = NULL;
            cells_to_solve_row_table[i][j] = NULL;
            cells_to_solve_column_table[i][j] = NULL;
            cells_to_solve_box_table_copy[i][j] = NULL;
        }
    }
}

void set_difficulty(int chosen_difficulty)
{
    switch (chosen_difficulty) {
        case 1:
            digits_to_remove = 27;
            break;
        case 2:
            digits_to_remove = 34;
            break;
        case 3:
            digits_to_remove = 41;
            break;
        default:
            digits_to_remove = 27;
    }
}

int get_number_of_removed_digits()
{
    return digits_to_remove;
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
            increase_free_numbers(row, col);
            Cell_to_solve_node *node = malloc(sizeof(Cell_to_solve_node));
            fill_cell(node, row, col);
            sudoku_grid_copy[row][col] = 0;

            remaining_digits--;
        }
    } while (remaining_digits > 0);
}

void fill_cell(Cell_to_solve_node *cell, int row, int col)
{
    cell->row = row;
    cell->col = col;
    cell->sudoku_box = sudoku_boxes[calculate_box(row, col)];
    cell->sudoku_row = sudoku_rows[row];
    cell->sudoku_column = sudoku_cols[col];
    cell->next = head;
    head = cell;
}

void fill_cell_tables()
{
    Cell_to_solve_node *current_cell = head;

    while(current_cell != NULL) {
        insert_in_box_table(current_cell, calculate_box(current_cell->row, current_cell->col));
        insert_in_row_table(current_cell, current_cell->row);
        insert_in_column_table(current_cell, current_cell->col);
        current_cell = current_cell->next;
    }
}

void insert_in_box_table(Cell_to_solve_node *cell, int index)
{
    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_box_table[index][i] == NULL) {
            cells_to_solve_box_table[index][i] = cell;
            break;
        }
    }
}

void insert_in_row_table(Cell_to_solve_node *cell, int index)
{

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_row_table[index][i] == NULL) {
            cells_to_solve_row_table[index][i] = cell;
            break;
        }
    }
}

void insert_in_column_table(Cell_to_solve_node *cell, int index)
{

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_column_table[index][i] == NULL) {
            cells_to_solve_column_table[index][i] = cell;
            break;
        }
    }
}

void delete_cell_from_tables(int row, int col)
{
    int box_index = calculate_box(row, col);

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_box_table[box_index][i] != NULL) {
            if (cells_to_solve_box_table[box_index][i]->row == row && cells_to_solve_box_table[box_index][i]->col == col) {
                cells_to_solve_box_table[box_index][i] = NULL;
                if (i > 0) {
                    if (i == 8) {
                        break;
                    }
                    if (cells_to_solve_box_table[box_index][i+1] == NULL) {
                        break;
                    }
                }

                for (int j = i; j < 9; j++) {
                    if (cells_to_solve_box_table[box_index][j+1] == NULL) {
                        cells_to_solve_box_table[box_index][j] = NULL;
                        break;
                    }
                    cells_to_solve_box_table[box_index][j] = cells_to_solve_box_table[box_index][j+1];
                }
            }
        }

    }

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_row_table[row][i] != NULL) {
            if (cells_to_solve_row_table[row][i]->row == row && cells_to_solve_row_table[row][i]->col == col) {
                cells_to_solve_row_table[row][i] = NULL;
                if (i > 0) {
                    if (i == 8) {
                        break;
                    }
                    if (cells_to_solve_row_table[row][i+1] == NULL) {
                        break;
                    }
                }

                for (int j = i; j < 9; j++) {
                    if (cells_to_solve_row_table[row][j+1] == NULL) {
                        cells_to_solve_row_table[row][j] = NULL;
                        break;
                    }
                    cells_to_solve_row_table[row][j] = cells_to_solve_row_table[row][j+1];
                }
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_column_table[col][i] != NULL) {
            if (cells_to_solve_column_table[col][i]->row == row && cells_to_solve_column_table[col][i]->col == col) {
                cells_to_solve_column_table[col][i] = NULL;
                if (i > 0) {
                    if (i == 8) {
                        break;
                    }
                    if (cells_to_solve_column_table[col][i+1] == NULL) {
                        break;
                    }
                }

                for (int j = i; j < 9; j++) {
                    if (cells_to_solve_column_table[col][j+1] == NULL) {
                        cells_to_solve_column_table[col][j] = NULL;
                        break;
                    }
                    cells_to_solve_column_table[col][j] = cells_to_solve_column_table[col][j+1];
                }
            }
        }
    }
}


int try_to_insert_number_in_box(int index)
{

    if (cells_to_solve_box_table[index][0] != NULL) {
        if (cells_to_solve_box_table[index][0]->sudoku_box->free_numbers == 1) {
            for (int i = 0; i < 9; i++) {
                if (cells_to_solve_box_table[index][0]->sudoku_box->numbers[i] == 0) {
                    insert_number(i+1, cells_to_solve_box_table[index][0]->row, cells_to_solve_box_table[index][0]->col);
                    sudoku_grid_copy[cells_to_solve_box_table[index][0]->row][cells_to_solve_box_table[index][0]->col] = i+1;
                    delete_cell_from_tables(cells_to_solve_box_table[index][0]->row,cells_to_solve_box_table[index][0]->col);
                    return 1;
                }
            }
        }
    }

    int number_of_inserted_cells = 0;
    int row_to_insert = 0;
    int col_to_insert = 0;
    for (int i = 1; i <= 9; i++) {
        if (cells_to_solve_box_table[index][0] == NULL) {
            return number_of_inserted_cells;
        }


        if (cells_to_solve_box_table[index][0] != NULL) {
            if (cells_to_solve_box_table[index][0]->sudoku_box->numbers[i-1] == i) {
                continue;
            }
        }


        int number = i;
        int number_of_possibilities = 0;

        for (int j = 0; j < 9; j++) {
            if (cells_to_solve_box_table[index][j] != NULL) {
                int row = cells_to_solve_box_table[index][j]->row;
                int col = cells_to_solve_box_table[index][j]->col;


                if (!check_on_duplication(number, row, col)) {
                    number_of_possibilities++;
                    if (number_of_possibilities > 1) {

                        j = 9;
                        continue;
                    }

                    row_to_insert = row;
                    col_to_insert = col;
                }
            }
        }

        if (number_of_possibilities == 1) {
            insert_number(number, row_to_insert, col_to_insert);
            sudoku_grid_copy[row_to_insert][col_to_insert] = number;
            delete_cell_from_tables(row_to_insert, col_to_insert);
            number_of_inserted_cells++;
        }
    }

    return number_of_inserted_cells;
}


int solve_sudoku(int remaining_digits)
{
    if (remaining_digits == 0) {
        return 1;
    }
    int remaining_digits_at_start = remaining_digits;

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_box_table[i][0] == NULL) {
            continue;
        }
        remaining_digits = remaining_digits - try_to_insert_number_in_box(calculate_box(cells_to_solve_box_table[i][0]->row, cells_to_solve_box_table[i][0]->col));
    }

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_row_table[i][0] == NULL) {
            continue;
        }
        remaining_digits = remaining_digits - try_to_insert_number_in_row(cells_to_solve_row_table[i][0]->row);
    }

    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_column_table[i][0] == NULL) {
            continue;
        }
        remaining_digits = remaining_digits - try_to_insert_number_in_column(cells_to_solve_column_table[i][0]->col);
    }

    if (remaining_digits_at_start == remaining_digits) {
        return 0;
    } else {
        if (solve_sudoku(remaining_digits)) {
            return 1;
        }
    }
    return 0;
}


int try_to_insert_number_in_row(int index)
{
    if (cells_to_solve_row_table[index][0] != NULL) {
        if (cells_to_solve_row_table[index][0]->sudoku_row->free_numbers == 1) {
            for (int i = 0; i < 9; i++) {
                if (cells_to_solve_row_table[index][0]->sudoku_row->numbers[i] == 0) {
                    insert_number(i+1, cells_to_solve_row_table[index][0]->row, cells_to_solve_row_table[index][0]->col);
                    sudoku_grid_copy[cells_to_solve_row_table[index][0]->row][cells_to_solve_row_table[index][0]->col] = i+1;
                    delete_cell_from_tables(cells_to_solve_row_table[index][0]->row, cells_to_solve_row_table[index][0]->col);
                    return 1;
                }
            }
        }
    }


    int number_of_inserted_cells = 0;
    int row_to_insert = 0;
    int col_to_insert = 0;
    for (int i = 1; i <= 9; i++) {
        if (cells_to_solve_row_table[index][0] == NULL) {
            return number_of_inserted_cells;
        }

        if (cells_to_solve_row_table[index][0]->sudoku_row->numbers[i-1] == i) {
            continue;
        }

        int number = i;
        int number_of_possibilities = 0;

        for (int j = 0; j < 9; j++) {
            if (cells_to_solve_row_table[index][j] != NULL) {
                int row = cells_to_solve_row_table[index][j]->row;
                int col = cells_to_solve_row_table[index][j]->col;


                if (!check_on_duplication(number, row, col)) {
                    number_of_possibilities++;
                    if (number_of_possibilities > 1) {
                        j = 9;
                        continue;
                    }
                    row_to_insert = row;
                    col_to_insert = col;
                }
            }
        }

        if (number_of_possibilities == 1) {
            insert_number(number, row_to_insert, col_to_insert);
            sudoku_grid_copy[row_to_insert][col_to_insert] = number;
            delete_cell_from_tables(row_to_insert, col_to_insert);
            number_of_inserted_cells++;
        }
    }

    return number_of_inserted_cells;
}

int try_to_insert_number_in_column(int index)
{
    if (cells_to_solve_column_table[index][0] != NULL) {
        if (cells_to_solve_column_table[index][0]->sudoku_column->free_numbers == 1) {
            for (int i = 0; i < 9; i++) {
                if (cells_to_solve_column_table[index][0]->sudoku_column->numbers[i] == 0) {
                    insert_number(i+1, cells_to_solve_column_table[index][0]->row, cells_to_solve_column_table[index][0]->col);
                    sudoku_grid_copy[cells_to_solve_column_table[index][0]->row][cells_to_solve_column_table[index][0]->col] = i+1;
                    delete_cell_from_tables(cells_to_solve_column_table[index][0]->row, cells_to_solve_column_table[index][0]->col);
                    return 1;
                }
            }
        }
    }


    int number_of_inserted_cells = 0;
    int row_to_insert = 0;
    int col_to_insert = 0;
    for (int i = 1; i <= 9; i++) {
        if (cells_to_solve_column_table[index][0] == NULL) {
            return number_of_inserted_cells;
        }

        if (cells_to_solve_column_table[index][0]->sudoku_column->numbers[i-1] == i) {
            continue;
        }

        int number = i;
        int number_of_possibilities = 0;

        for (int j = 0; j < 9; j++) {
            if (cells_to_solve_column_table[index][j] != NULL) {
                int row = cells_to_solve_column_table[index][j]->row;
                int col = cells_to_solve_column_table[index][j]->col;


                if (!check_on_duplication(number, row, col)) {
                    number_of_possibilities++;
                    if (number_of_possibilities > 1) {
                        j = 9;
                        continue;
                    }
                    row_to_insert = row;
                    col_to_insert = col;
                }
            }
        }

        if (number_of_possibilities == 1) {
            insert_number(number, row_to_insert, col_to_insert);
            sudoku_grid_copy[row_to_insert][col_to_insert] = number;
            delete_cell_from_tables(row_to_insert, col_to_insert);
            number_of_inserted_cells++;
        }
    }

    return number_of_inserted_cells;
}

/* Function to delete the entire linked list */
void deleteList()
{
    /* deref head_ref to get the real head */
    Cell_to_solve_node *current = head;
    Cell_to_solve_node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    /* deref head_ref to affect the real head back
       in the caller. */
    head = NULL;
}

void copy_box_table()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cells_to_solve_box_table_copy[i][j] = cells_to_solve_box_table[i][j];
        }
    }
}

int check_if_cell_was_deleted(int row, int col)
{
    int box_index = calculate_box(row, col);
    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_box_table_copy[box_index][i] == NULL) {
            return 0;
        }

        if (cells_to_solve_box_table_copy[box_index][i]->row == row && cells_to_solve_box_table_copy[box_index][i]->col == col) {
            return 1;
        }
    }

    return 0;
}

int check_if_sudoku_is_valid() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku_grid[i][j] != sudoku_grid_copy[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}



