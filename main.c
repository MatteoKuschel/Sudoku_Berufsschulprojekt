#include "SudokuLibrary/library.h"
#include <stdio.h>

int main() {
    initialize_structs();
    int test = fill_grid();
    int check_if_solvable = 1;
    set_difficulty(3);
    int numbers = get_number_of_removed_digits();
    do {
        initialize_tables();
        copy_sudoku_grid();
        remove_sudoku_grid_numbers();
        fill_cell_tables();
        check_if_solvable = solve_sudoku(numbers);

        if (!check_if_solvable) {
            deleteList();
        }
    } while (!check_if_solvable);

    print_sudoku();
}
