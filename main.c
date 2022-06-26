#include "SudokuLibrary/library.h"
#include <stdio.h>

// Falls library.h nicht gefunden wird, müssen library.h und library.c in den selben Ordner mit der main.c gelegt werden 
// und das include in der main.c muss auf #include library.h geändert werden. 

int main() {
    initialize_structs();
    
    // print_sudoku();
    play_game();
    // int test = fill_grid();
    // int check_if_solvable = 1;
    // set_difficulty(3);
    // int numbers = get_number_of_removed_digits();
    // do {
    //     initialize_tables();
    //     copy_sudoku_grid();
    //     remove_sudoku_grid_numbers();
    //     fill_cell_tables();
    //     check_if_solvable = solve_sudoku(numbers);

    //     if (!check_if_solvable) {
    //         deleteList();
    //     }
    // } while (!check_if_solvable);

    // print_sudoku();
    
}
