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
} Sudoku_column;

Sudoku_row *sudoku_rows[9];
Sudoku_column *sudoku_cols[9];
Sudoku_box *sudoku_boxes[9];

static int sudoku_grid[9][9] = {0};

/**Gibt an in welcher Box von 0 bis 8 von oben rechts nach unten links man sich befindet*/
int calculate_box(int row, int col);

/**Überprüft ob die Nummer schon in der Box, der Spalte oder Reihe existiert.
 Gibt 1 zurück wenn Ja, 0 wenn falsch*/
int check_on_duplication(int number, int row, int col);

/**Fügt die Nummer in der jeweiligen Box, Spalte und Reihe ein*/
void insert_number(int number, int row, int col);

/**Löscht die Nummer aus der jeweiligen Box, Spalte und Reihe*/
void delete_number(int number, int row, int col);

/**Sortiert die Zahlen in dem Array zufällig um*/
void shuffle(int *array);

/**Füllt ein 2 dimensionalen Array solange mit zufälligen Zahlen bis ein vollständiges Sudoku Feld entsteht*/
int fill_grid();

/**Gibt das Sudoku Grid zum testen in der Console aus.*/
void print_sudoku();

/** Initialisiert die Strukturen*/
void initialize_structs();

//Funktion, um den aktuellen Spielstand in einer CSV-Datei zu speichern
void save_score_data(int *sudoku[9][9], char filename[50]);

