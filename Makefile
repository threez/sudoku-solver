CC=gcc
FLAGS=-Wall -O2

sudoku: sudoku.c
	${CC} ${FLAGS} sudoku.c -o sudoku

test: sudoku
	./sudoku *.txt
