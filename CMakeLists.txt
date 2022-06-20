cmake_minimum_required(VERSION 3.20)
project(Sudoku C)

set(CMAKE_C_STANDARD 99)

add_executable(Sudoku main.c SudokuLibrary/library.h)
