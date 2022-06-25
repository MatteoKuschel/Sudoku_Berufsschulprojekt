#include "SudokuLibrary/library.h"
#include <stdio.h>

int main() {
    initialize_structs();
    int test = fill_grid();
    copy_sudoku_grid();
    // print_sudoku();
    play_game();
    
}
