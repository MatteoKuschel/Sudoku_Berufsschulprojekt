#include "library.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>

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

void insert_digits_to_in_box_copy_after_solving()
{
    Cell_to_solve_node *current_cell = head;

    while(current_cell != NULL) {
        insert_in_box_table_copy(current_cell, calculate_box(current_cell->row, current_cell->col));
        current_cell = current_cell->next;
    }
}

void insert_in_box_table_copy(Cell_to_solve_node *cell, int index)
{
    for (int i = 0; i < 9; i++) {
        if (cells_to_solve_box_table_copy[index][i] == NULL) {
            cells_to_solve_box_table_copy[index][i] = cell;
            break;
        }
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

void deleteList()
{
    Cell_to_solve_node *current = head;
    Cell_to_solve_node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

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

double calculate_game_time(clock_t start_clock)
{
    //Variablen initialisieren
    clock_t current_clock;
    double current_time;

    //Aktuelle Zeit ermitteln
    current_clock = clock();

    //Differenz aus der aktuellen Zeit und der Anfangszeit ermitteln und zurückgeben
    current_time = (current_clock - start_clock)/CLOCKS_PER_SEC;
    return (current_time + 4208);
}

void print_game_time(double current_time)
{
    int hours, minutes, seconds;

    //Sekunden, Minuten und Stunden ermitteln
    hours = current_time/3600;
    current_time-=  hours*3600;

    minutes = current_time/60;
    current_time -= minutes*60;

    seconds = current_time;

    printf("Spielzeit: %i:%i:%i",hours,minutes,seconds);
}

//Funktion, die das Sudoku aus einer CSV_Datei einliest
void get_sudoku_from_csv_file(char filename[50])
{
    //Deklaration der Variablen
    FILE *sudoku_file;
    char puffer[20];
    int line, column;

    //CSV-Datei im "Read"-Modus öffnen
    sudoku_file = fopen(filename,"r");

    //Überprüfung, ob die Datei gefunden wurde
    if(sudoku_file == NULL)
    {
        printf("Sudoku wurde nicht gefunden");
    }

    //Zeilen der CSV-Datei einlesen und die Werte dem Sudoku-Array zuweisen
    line = 0;
    while(fgets(puffer,20,sudoku_file))
    {
        for(column=0;column<9;column++)
        {
            sudoku_grid_copy[line][column] =  puffer[column*2]-48;
        }
        line++;
    }
}

void save_score_data(char filename[50])
{
    //Deklaration der Variablen
    FILE *sudoku_file;
    int line , column, first_value;

    //Datei im "Write"-Modus öffnen
    sudoku_file = fopen(filename,"w");

    //Werte aus dem Array in die CSV-Datei schreiben
    for(line=0;line<9;line++)
    {
        first_value = 1;
        for(column=0;column<9;column++)
        {
            if (first_value == 1)
            {
                fprintf(sudoku_file,"%i", sudoku_grid_copy[line][column]);
                first_value = 0;
            }else
            {
                fprintf(sudoku_file,";%i", sudoku_grid_copy[line][column]);
            }

        }
        fprintf(sudoku_file,"\n");

    }
    //Datei schließen
    fclose(sudoku_file);
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

//Setzt den Cusor im Terminal. (Genommen von Konsolen Demo)
int set_cursor(int x, int y)
{
    COORD koordinaten;
    koordinaten.X= x;
    koordinaten.Y= y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinaten);
    return 0;
}

//Gibt eine Box im Terminal aus. (Genommen von Konsolen Demo)
void print_box(int HOEHE, int BREITE)
{
    for (int i = 0; i < HOEHE; i++)
    {
        for (int j = 0; j < BREITE; j++)
        {
            if (i == 0 && j == 0)
                printf("\xC9");
            else if (i == 0 && j == BREITE-1)
                printf("\xBB");
            else if (i == HOEHE-1 && j == 0)
                printf("\xC8");
            else if (i == HOEHE-1 && j == BREITE-1)
                printf("\xBC");
            else if (i == 0 || i == HOEHE-1)
                printf("\xCD");
            else if (j == 0 || j == BREITE-1)
                printf("\xBA");
            else
                printf(" ");
        }
        printf("\n");
    }
}

//Gibt ein Text aus, der einen darauf hinweißt, dass man mit der Eingabe von x das Spiel beenden kann.
void print_quit_game_text() {
    system("cls");
    print_box(6,40);
    set_cursor(8,1);
    set_cursor(8,2);
    printf("Spiel beenden: x \n");
    set_cursor(8,3);
    printf("Spielstand speichern: s \n");
}

// Gibt einem die Möglichkeit den Schwierigkeitsgrad des Spieles auszuwählen.
void set_difficulty_of_game() {
    int difficulty;
    char difficulty_c;

    system("cls");
    print_box(10,50);
    set_cursor(9,1);
    printf("Spielschwierigkeit einstellen: \n");
    set_cursor(6,2);
    printf("");
    set_cursor(6,3);
    printf("Leicht: 1\n");
    set_cursor(6,4);
    printf("Mittel: 2\n");
    set_cursor(6,5);
    printf("Schwer: 3\n\n");
    set_cursor(6,6);
    printf("");
    set_cursor(6,7);
    printf("");

    // Spielereingabe für die Wahl des Schwierigkeitsgrades.
    while (1)
    {
        printf("Wahl: ");
        scanf(" %c",&difficulty_c);

        // Verändert das Spielfeld je nach Eingabe des Schwierigkeitgrades.
        if ((difficulty_c == '1') || (difficulty_c == '2') || (difficulty_c == '3')){

            difficulty = (int)difficulty_c - 48;
            set_difficulty(difficulty);
            remove_sudoku_grid_numbers();
            break;
        } else {
            printf("\nUng\x81ltige eingabe!\n");
        }
    }
}

// Gibt das Sudoku Spiel im Terminal aus.
void print_current_sudoku_grid(clock_t start_clock)
{
    double game_time;

    printf("\n\n");
    game_time = calculate_game_time(start_clock);
    print_game_time(game_time);

    printf("\n --------- --------- ---------\n");
    for (int l = 0; l < 9; l++) {
        printf("|");

        for (int m = 0; m < 9; m++) {
            if ((m == 2) | (m == 5) | (m == 8)){

                if (sudoku_grid_copy[l][m] == 0) {
                    printf("   |");
                } else {
                    printf(" %i |",sudoku_grid_copy[l][m]);
                }

            } else {

                if (sudoku_grid_copy[l][m] == 0) {
                    printf("   ");
                } else {
                    printf(" %i ",sudoku_grid_copy[l][m]);
                }
            }

            if ((m == 8) & (l == 2) | (m == 8) & (l == 5) | (m == 8) & (l == 8)) {
                printf("\n --------- --------- ---------");
            }
        }
        printf("\n");
    }
}

// Ändert den Wert eines Sudoku Feldes anhand der Spielereingabe.
void set_sudoku_grid_field(int row, int col, int value)
{
    int previous_value = sudoku_grid_copy[row][col];
    sudoku_grid_copy[row][col] = value;
    if (digits_to_remove == 0) {
        return;
    }

    if (previous_value == 0) {
        digits_to_remove--;
    }
}

// Bietet einem die Möglichkeit ein Spielstand zu laden.
void set_save_file(){
    char load_save_file;
    char save_file[50];

    print_box(6,40);
    set_cursor(9,1);

    while (1)
    {
        printf("Spielstand laden? y/n\n ");
        printf("Um einen Spielstand zu laden muss eine CSV Datei im Ordner existieren, die Spielstand.csv heißt");
        scanf(" %c",&load_save_file);
        printf("\n");

        if ((load_save_file == 'y') || (load_save_file == 'Y')) {
            get_sudoku_from_csv_file("Spielstand.csv");
            break;
        }
        if ((load_save_file == 'n') || (load_save_file == 'N')){
            initialize_structs();
            int test = fill_grid();
            copy_sudoku_grid();
            int check_if_solvable = 1;
            set_difficulty_of_game();
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
            insert_digits_to_in_box_copy_after_solving();
            remove_cells_from_grid_after_solving();
            break;
        }

        printf("\nUng\x94ltige eingabe!\n");
    }
}

void remove_cells_from_grid_after_solving()
{
    Cell_to_solve_node *current_cell = head;

    while(current_cell != NULL) {
        int row = current_cell->row;
        int col = current_cell->col;
        delete_number(sudoku_grid_copy[row][col], row, col);
        increase_free_numbers(row, col);
        sudoku_grid_copy[row][col] = 0;

        current_cell = current_cell->next;
    }
}

void play_game()
{
    int row;
    int col;
    int value;
    char row_c;
    char col_c;
    char value_c;
    char save_file[50];

    clock_t start_clock = clock;

    system("cls");
    set_save_file();
    print_quit_game_text();

    while (1)
    {
        print_current_sudoku_grid(start_clock);
        printf("\n");

        // Spielereingabe für die Reihe.
        while (1)
        {
            printf("Reihe: \n");
            scanf(" %c",&row_c);

            if ((row_c  >= '1') && (row_c <= '9')) {
                break;
            }

            if ((row_c == 'x') || (row_c == 'X')) {
                printf("Spiel beendet!");
                printf("\n");
                return;
            }

            if ((row_c == 's') || (row_c == 'S')) {
                save_score_data("Spielstand.csv");
                printf("\n");
                return;
            }
            printf("\nUng\x81ltige eingabe!\n");
        }

        // Spielereingabe für die Spalte.
        while (1)
        {
            printf("Spalte: \n");
            scanf(" %c",&col_c);

            if ((col_c  >= '1') && (col_c <= '9')) {
                break;
            }

            //Bricht das Spiel ab, falls der Spieler in einen der Eingaben ein kleines oder großes X eingibt.
            if ((col_c == 'x') || (col_c == 'X')) {
                printf("Spiel beendet!");
                printf("\n");
                return;
            }
            if ((col_c == 's') || (col_c == 'S')) {
                // printf("Gib den Namen der Save-file an: \n");
                // scanf(" %c",&save_file);
                save_score_data("Spielstand.csv");
                printf("\n");
                return;
            }
            printf("\nUng\x81ltige eingabe!\n");
        }

        // Spielereinagbe um 1 subtrahiert für besseres/intuitives Spielerlebnis.
        row = (int)row_c - 49;
        col = (int)col_c - 49;

        // Verhindert, dass ein Feld geändert wird, welches bereits ein Wert hat
        if (!check_if_cell_was_deleted(row, col)) {
            printf("Zelle kann nicht verändert werden");
            continue;
        }
        // Spielereingabe für den Eingabewert.
        while (1)
        {
            printf("Wert: \n");
            scanf(" %c",&value_c);

            if ((value_c  >= '1') && (value_c <= '9')) {
                break;
            }
            //Bricht das Spiel ab, falls der Spieler in einen der Eingaben ein kleines oder großes X eingibt.
            if ((value_c == 'x') || (value_c == 'X')){
                printf("Spiel beendet!");
                printf("\n");
                return;
            }
            if ((value_c == 's') || (value_c == 'S')) {
                // printf("Gib den Namen der Save-file an: \n");
                // scanf(" %c",&save_file);
                save_score_data("Spielstand.csv");
                printf("\n");
                return;
            }
            printf("\nUng\x81ltige eingabe!\n");

        }

        value = (int)value_c - 48;
        set_sudoku_grid_field(row,col,value);

        if (digits_to_remove == 0){
            printf("\n");
            // Funktion überprüfen, ob man gewonnnen hat oder nicht.
            if (check_if_sudoku_is_valid()) {
                printf("Spiel gewonnen!");
                break;
            }
            printf("Sudoku ist falsch!");
        }
    }
}




