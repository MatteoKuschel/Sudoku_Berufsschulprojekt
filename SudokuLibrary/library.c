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

//Funktion, die das Sudoku aus einer CSV_Datei einliest
get_sudoku_from_csv_file(int *sudoku[9][9], char filename[50])
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
            sudoku[line][column] =  puffer[column*2]-48;
        }
        line++;
    }

save_score_data(int *sudoku[9][9], char filename[50])
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
                fprintf(sudoku_file,"%i", sudoku[line][column]);
                first_value = 0;
            }else
            {
                fprintf(sudoku_file,";%i", sudoku[line][column]);
            }

        }
        fprintf(sudoku_file,"\n");

    }
    //Datei schließen
    fclose(sudoku_file);
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

//Gibt ein Text aus, der einen darauf hinweißt, dass man mit der Eingabe von x das Spiel beenden kann.
void print_quit_game_text() {

    printf("\n\n");
    printf("//////////////////////\n");
    printf("/   Spiel beenden: x /\n");
    printf("//////////////////////\n");
}

// Gibt einem die Möglichkeit den Schwierigkeitsgrad des Spieles auszuwählen.
void set_difficulty_of_game() {
    char difficulty;

    printf("\n\n");
    printf("Spielschwierigkeit einstellen: \n");
    printf("Leicht: 1\n");
    printf("Mittel: 2\n");
    printf("Schwer: 3\n\n");

    // Spielereingabe für die Wahl des Schwierigkeitsgrades.
    while (1)
    {
        printf("Wahl: ");
        scanf(" %c",&difficulty);

        if ((difficulty == '1') || (difficulty == '2') || (difficulty == '3')){
            break;
        } else {
            printf("\nUngueltige eingabe!\n");
        }
    }

    // Verändert das Spielfeld je nach Eingabe des Schwierigkeitgrades. 
    switch (difficulty)
    {
    case '1' : 
        // Stellt das Spiel auf leicht ein.
        set_difficulty(1);
        remove_sudoku_grid_numbers(); 
        break;
    case '2' : 
        // Stellt das Spiel auf mittel ein.
        set_difficulty(2);
        remove_sudoku_grid_numbers();
        break;
    case '3' : 
        // Stellt das Spiel auf schwer ein.
        set_difficulty(3);
        remove_sudoku_grid_numbers();
        break;
    
    default:
        break;
    }
}

// Gibt das Sudoku Spiel im Terminal aus.
void print_current_sudoku_grid()
{
    printf("\n\n");
    printf("\n --------- --------- ---------\n");

    for (int l = 0; l < 9; l++) {
        printf("|"); 

        for (int m = 0; m < 9; m++) {
            if ((m == 2) | (m == 5) | (m == 8)){
                
                if (sudoku_grid[l][m] == 0) {
                    printf("   |");
                } else {
                     printf(" %i |",sudoku_grid[l][m]);
                }
            
            } else {

                if (sudoku_grid[l][m] == 0) {
                    printf("   ");
                } else {
                    printf(" %i ",sudoku_grid[l][m]);   
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
int set_sudoku_grid_field(int row, int col, int value)
{
    sudoku_grid[row][col] = value;
}

// Gibt das aktuelle Spielfeld aus und bietet einem die Möglickeit die Werte der Felder zu verändern.
int play_game()
{
    int row;
    int col;
    int value;
    char row_c;
    char col_c;
    char value_c;

    print_quit_game_text();
    set_difficulty_of_game();
    
    while (1)
    {
        print_current_sudoku_grid();
        printf("\n");

        // Spielereingabe für die Reihe.
        while (1)
        {
            printf("Reihe: \n");
            scanf(" %c",&row_c);

            if ((row_c  >= '0') && (row_c <= '9') || (row_c == 'x') || (row_c == 'X')) {
                break;
            } else {
            printf("\nUngueltige eingabe!\n");
        }
        }

        // Spielereingabe für die Spalte.
        while (1)
        {
            printf("Spalte: \n");
            scanf(" %c",&col_c);

            if ((col_c  >= '0') && (col_c <= '9') || (col_c == 'x') || (col_c == 'X')) {
                break;
            } else {
            printf("\nUngueltige eingabe!\n");
        }
        }

        // Spielereingabe für den Eingabewert.
        while (1)
        {
            printf("Wert: \n");
            scanf(" %c",&value_c);

            if ((value_c  >= '0') && (value_c <= '9') || (value_c == 'x') || (value_c == 'X')) {
                break;
            } else {
            printf("\nUngueltige eingabe!\n");
        }
        }

        //Bricht das Spiel ab, falls der Spieler in einen der Eingaben ein kleines oder großes X eingibt.
        if (
            !((row_c  >= '0') && (row_c <= '9')) || 
            !((col_c  >= '0') && (col_c <= '9')) || 
            !((value_c  >= '0') && (value_c <= '9'))
            )
        {
            printf("\n");
            printf("Spiel beendet!");
            printf("\n");
            break;
        }

        row = (int)row_c - 48;
        col = (int)col_c - 48;
        value = (int)value_c - 48;

        // Spielereinagbe um 1 subtrahiert für besseres/intuitives Spielerlebnis.
        row = row - 1;
        col = col - 1;
        
        set_sudoku_grid_field(row,col,value);
    }
}

