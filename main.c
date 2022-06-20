#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9

typedef struct Sudoku_box
{
    int free_numbers;
    int numbers[9];
} Sudoku_box;

typedef struct Sudoku_row
{
    int free_numbers;
    int numbers[9];
} Sudoku_row;

typedef struct Sudoku_colum
{
    int free_numbers;
    int numbers[9];
} Sudoku_colum;

Sudoku_row *sudoku_rows[N];
Sudoku_colum *sudoku_cols[N];
Sudoku_box *sudoku_boxes[N];
int sudoku_grid[9][9] = {0};

/*Gibt an in welcher Box von 0 bis 8 von oben rechts nach unten links man sich befindet*/
int calculate_box(int row, int col)
{
    return ((int) row / 3) * 3 + ((int) col / 3);
}

/*Überprüft ob die Nummer schon in der Box, der Spalte oder Reihe existiert.
 * Gibt 1 zurück wenn Ja, 0 wenn falsch*/
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

/*Fügt die Nummer in der jeweiligen Box, Spalte und Reihe ein*/
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

/*Löscht die Nummer aus der jeweiligen Box, Spalte und Reihe*/
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

/*Sortiert die Zahlen in dem Array zufällig um*/
void shuffle(int *array)
{
    srand(time(0));
    for (int i = 0; i < N - 1; i++) {
        int j = i + rand() / (RAND_MAX / (N - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

/*Füllt ein 2 dimensionalen Array solange mit zufälligen Zahlen bis ein vollständiges Sudoku Feld entsteht*/
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

int main() {
    for (int i = 0; i < N; ++i) {
        sudoku_boxes[i] = malloc(sizeof(Sudoku_box));
        sudoku_cols[i] = malloc(sizeof(Sudoku_colum));
        sudoku_rows[i] = malloc(sizeof(Sudoku_row));

        sudoku_boxes[i]->free_numbers = 9;
        sudoku_cols[i]->free_numbers = 9;
        sudoku_rows[i]->free_numbers = 9;

        for (int j = 0; j < N; ++j) {
            sudoku_boxes[i]->numbers[j] = 0;
            sudoku_cols[i]->numbers[j] = 0;
            sudoku_rows[i]->numbers[j] = 0;
        }
    }

    int test = fill_grid();
    printf("\n\n");
    printf("GRID-------------\n");
    for (int l = 0; l < N; l++) {
        for (int m = 0; m < N; m++) {
            printf("%i ",sudoku_grid[l][m]);
        }
        printf("\n");
    }
}
