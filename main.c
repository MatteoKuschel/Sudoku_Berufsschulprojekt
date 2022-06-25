#include "SudokuLibrary/library.h"
#include <stdio.h>

int main() {
    initialize_structs();
    int test = fill_grid();
    copy_sudoku_grid();
    print_sudoku();
    printf("\n");
    set_difficulty(3);
    remove_sudoku_grid_numbers();
    print_sudoku();

}
