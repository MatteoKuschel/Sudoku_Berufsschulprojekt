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

typedef struct Sudoku_column
{
    int free_numbers;
    int numbers[9];
} Sudoku_column;

typedef struct Cell_to_solve_node
{
    int row;
    int col;
    Sudoku_box *sudoku_box;
    Sudoku_row *sudoku_row;
    Sudoku_column *sudoku_column;
    struct Cell_to_solve_node *next;
} Cell_to_solve_node;


Cell_to_solve_node *cells_to_solve_box_table[9][9];
Cell_to_solve_node *cells_to_solve_row_table[9][9];
Cell_to_solve_node *cells_to_solve_column_table[9][9];
Cell_to_solve_node *cells_to_solve_box_table_copy[9][9];

Sudoku_row *sudoku_rows[9];
Sudoku_column *sudoku_cols[9];
Sudoku_box *sudoku_boxes[9];

Sudoku_row *sudoku_rows_to_solve[9];
Sudoku_column *sudoku_columns_to_solve[9];
Sudoku_box *sudoku_boxes_to_solve[9];


static int digits_to_remove = 0;
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

/** Legt fest wie viele Zahlen aus dem Grid gelöscht werden. Es gibt drei verschiedene Schwierigkeitsgrade von 1 bis 3.*/
void set_difficulty(int chosen_difficulty);

/** Kopiert die Zahlen in ein neues grid*/
void copy_sudoku_grid();

/** Löscht, je nach Schwierigkeitsgrad verschiedend viele Zahlen aus dem Grid.*/
void remove_sudoku_grid_numbers();

/** Füllt die Zelle/Node mit Werten*/
void fill_cell(Cell_to_solve_node *node, int row, int col);

/** Füllt die Tabellen für die gelöschten Zellen/Nodes mit Zellen/Nodes*/
void fill_cell_tables();

/** Versucht das generierte Sudoku auf menschlicher Art zu lösen. Gibt 1 zurück, wenn es lösbar ist, 0, wenn nicht*/
int solve_sudoku(int remaining_digits);

/** Erhöht die free_numbers Variable um 1*/
void increase_free_numbers(int row, int col);

/** Gibt die Anzahl an gelöschten Variabeln zurück*/
int get_number_of_removed_digits();

/** Füllt die Box mit der Zelle an dem gegebenen index*/
void insert_in_box_table(Cell_to_solve_node *cell, int index);

/** Füllt die Reihe mit der Zelle an dem gegebenen index*/
void insert_in_row_table(Cell_to_solve_node *cell, int index);

/** Füllt die Spalte mit der Zelle an dem gegebenen index*/
void insert_in_column_table(Cell_to_solve_node *cell, int index);

/** Probiert die Zahlen von 1 - 9 für jede freie Zelle in der Box einzusetzen, wenn bei einer Zahl nur eine mögliche Zelle besteht wird diese gefüllt*/
int try_to_insert_number_in_box(int index);

/** Probiert die Zahlen von 1 - 9 für jede freie Zelle in der Reihe einzusetzen, wenn bei einer Zahl nur eine mögliche Zelle besteht wird diese gefüllt*/
int try_to_insert_number_in_row(int index);

/** Probiert die Zahlen von 1 - 9 für jede freie Zelle in der Spalte einzusetzen, wenn bei einer Zahl nur eine mögliche Zelle besteht wird diese gefüllt*/
int try_to_insert_number_in_column(int index);

/** Löscht die Zelle aus den Tabellen, nachdem diese befüllt wurde*/
void delete_cell_from_tables(int row, int col);

/** Löscht LinkedList für die Zellen*/
void deleteList();

/** Initializiert alle Tabellen mit NULL*/
void initialize_tables();

/** Kopiert die Werte aus der Box Tabelle*/
void copy_box_table();

/** Überprüft, ob das gelöste Spiel Grid mit der Lösung übereinstimmt*/
int check_if_sudoku_is_valid();

/** Überprüft, ob die ausgewählte Zelle vorher gelöscht wurde*/
int check_if_cell_was_deleted(int row, int col);