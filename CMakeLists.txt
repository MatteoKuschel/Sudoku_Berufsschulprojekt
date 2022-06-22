cmake_minimum_required(VERSION 3.20)
project(Sudoku_Berufsschulprojekt C)

set(CMAKE_C_STANDARD 99)

add_executable(Sudoku_Berufsschulprojekt main.c ./SudokuLibrary/library.c)
