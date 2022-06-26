#include "library.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>

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
double calculate_game_time(clock_t start_clock)
{
    //Variablen initialisieren
    clock_t current_clock;
    double current_time;

    //Aktuelle Zeit ermitteln
    current_clock = clock();

    //Differenz aus der aktuellen Zeit und der Anfangszeit ermitteln und zurückgeben
    current_time = (current_clock - start_clock)/CLOCKS_PER_SEC;
    return current_time;
}

void get_current_time(double current_time)
{
    int hours, minutes, seconds;
    
    //Sekunden, Minuten und Stunden ermitteln
    hours = current_time/3600;
    current_time-=  hours*3600;

    minutes = current_time/60;
    current_time -= minutes*60;

    seconds = current_time;

    printf("%i:%i:%i",hours,minutes,seconds);
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

int set_cursor(int x, int y)
{
    COORD koordinaten;
    koordinaten.X= x;
    koordinaten.Y= y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinaten);
    return 0;
}

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
void print_current_sudoku_grid()
{
    clock_t start_clock = clock;
    double game_time;
    printf("\n\n");
    game_time = calculate_game_time(start_clock);
    get_current_time(game_time);
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
    sudoku_grid_copy[row][col] = value;
    if (digits_to_remove == 0) {
        return;
    }
    digits_to_remove --;
}

// Bietet einem die Möglichkeit ein Spielstand zu laden.
void set_save_file(){
    char load_save_file;
    char save_file[50];

    
    print_box(6,40);
    set_cursor(9,1);

    while (1)
    {
        printf("Spielstand laden? y/n ");
        scanf(" %c",&load_save_file);
        printf("\n");

        if ((load_save_file == 'y') || (load_save_file == 'Y')) {
            set_cursor(9,2);
            printf("Gib den Namen der Save-file an: \n");
            set_cursor(9,2);
            printf("");
            set_cursor(9,3);
            scanf(" %c",&save_file);
            get_sudoku_from_csv_file("Spielstand.csv");
            break;
        }
        if ((load_save_file == 'n') || (load_save_file == 'N')){
            int test = fill_grid();
            copy_sudoku_grid();
            set_difficulty_of_game();
            break;
        }   
        printf("\nUng\x94ltige eingabe!\n");
    }
}

// Gibt das aktuelle Spielfeld aus und bietet einem die Möglickeit die Werte der Felder zu verändern.
void play_game()
{
    int row;
    int col;
    int value;
    char row_c;
    char col_c;
    char value_c;
    char save_file[50];
    
    system("cls");
    set_save_file();
    print_quit_game_text();
    int is_valid = 0;


    while (!is_valid)
    {

        print_current_sudoku_grid();
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
                // printf("Gib den Namen der Save-file an: \n");
                // scanf(" %c",&save_file);
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
        // if (sudoku_grid_copy[row][col] != 0) {
        //     printf("Feld ist nicht leer.");
        //     // Funktion: if 
        //     continue;
        // }

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
            // Funktion einbauen um zu überprüfen, ob man gewonnnen hat oder nicht. 
            // is_valid = funktionsaufruf
            printf("\n");
        }
    }
}

