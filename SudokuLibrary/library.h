

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

typedef struct Sudoku_box_node
{
    Sudoku_box *sudoku_box;
    struct Sudoku_box_node *next;
} Sudoku_box_node;

typedef struct Sudoku_row_node
{
    Sudoku_row *sudoku_row;
    struct Sudoku_row_node *next;
} Sudoku_row_node;

typedef struct Sudoku_column_node
{
    Sudoku_column *sudoku_column;
    struct Sudoku_column_node *next;
} Sudoku_column_node;

Sudoku_row *sudoku_rows[9];
Sudoku_column *sudoku_cols[9];
Sudoku_box *sudoku_boxes[9];

Sudoku_box_node *sudoku_box_nodes[9];
Sudoku_row_node *sudoku_row_nodes[9];
Sudoku_column_node *sudoku_column_nodes[9];

static int digits_to_remove = 0;

stat
static int sudoku_grid[9][9] = {0};
static int sudoku_grid_copy[9][9] = {0};

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

get_sudoku_from_csv_file(int *sudoku[9][9], char filename[50]);
//Funktion, um den aktuellen Spielstand in einer CSV-Datei zu speichern
save_score_data(int *sudoku[9][9], char filename[50]);

/** Legt fest wie viele Zahlen aus dem Grid gelöscht werden. Es gibt drei verschiedene Schwierigkeitsgrade von 1 bis 3.*/
void set_difficulty(int chosen_difficulty);

/** Kopiert die Zahlen in ein neues grid*/
void copy_sudoku_grid();

/** Löscht, je nach Schwierigkeitsgrad verschiedend viele Zahlen aus dem Grid.*/
void remove_sudoku_grid_numbers();

//Gibt ein Text aus, der einen darauf hinweißt, dass man mit der Eingabe von x das Spiel beenden kann.
void print_quit_game_text();

// Gibt einem die Möglichkeit den Schwierigkeitsgrad des Spieles auszuwählen.
void set_difficulty_of_game();

// Gibt das Sudoku Spiel im Terminal aus.
void print_current_sudoku_grid();

// Ändert den Wert eines Sudoku Feldes anhand der Spielereingabe.
int set_sudoku_grid_field(int row, int col, int value);

// Gibt das aktuelle Spielfeld aus und bietet einem die Möglickeit die Werte der Felder zu verändern.
int play_game();


