#include <stdlib.h>
#include <stdio.h>
#include "sudoku.h"

void sudoku_initialize(Sudoku *sudoku, const char* path) {
  FILE * f = fopen(path, "r");
  char c;
  short i = 0;
  while (feof(f) == 0 && i < FIELD_SIZE) {
    if ((c = fgetc(f)) != -1) {
      if (c >= '0' && c <= '9') {
        sudoku->grid[i] = ((short)c) - '0';
        sudoku->consts[i] = TRUE;
        i++;
      } else if (c == ' ') {
        sudoku->grid[i] = 0;
        sudoku->consts[i] = FALSE;
        i++;
      } else {
        // ignore
      }
    }
  }
  fclose(f);
}

short sudoku_solve(Sudoku *sudoku, const short i) {
  short n;  
  short * arr_of_nine;
  if (i == FIELD_SIZE) {
    return TRUE; // end reached
  } else if (sudoku->grid[i] == 0) {  
    arr_of_nine = (short *)malloc(sizeof(short) * ARC_SIZE);
    sudoku_possible(sudoku, arr_of_nine, sudoku_coord(i));
    for (n = 0; n < ARC_SIZE; ++n) {
      if (arr_of_nine[n] == 0) continue;
      sudoku->grid[i] = arr_of_nine[n];
       // sudoku_print(&sudoku);
       // printf("%s\033[1;1H");
      if (sudoku_solve(sudoku, i + 1) == TRUE) {
        free(arr_of_nine);
        return TRUE;
      }
    }
    sudoku->grid[i] = 0;
    free(arr_of_nine);
    return FALSE;
  } else {
    return sudoku_solve(sudoku, i + 1);
  }
}

void sudoku_print(Sudoku *sudoku) {
  short i;
  for (i = 0; i < FIELD_SIZE; ++i) {
    if (sudoku->grid[i] == 0) {
      printf("  ");
    } else if (sudoku->consts[i] == TRUE) {
      printf(" \033[34m%d\033[0m", sudoku->grid[i]);
    } else {
      printf(" %d", sudoku->grid[i]);
    }
    
    if ((i + 1) % 9 == 0 && i > 0) {
      printf(" \n");
      if (i / 9 < 8) {
        printf("---+---+---+---+---+---+---+---+---\n");
      }
    } else {
      printf(" |");
    }
  }
}

short sudoku_pos(const short x, const short y) {
  return y * ARC_SIZE + x;
}

Coordinate sudoku_coord(const short i) {
  Coordinate c;
  c.x = i % ARC_SIZE;
  c.y = (i - c.x) / 9;
  return c;
}

void sudoku_possible(Sudoku *sudoku, short * arr_of_nine, const Coordinate c) {
  short i;
  for (i = 0; i < ARC_SIZE; ++i)
    arr_of_nine[i] = i + 1;
  sudoku_horizontal(sudoku, arr_of_nine, c);
  sudoku_vertical(sudoku, arr_of_nine, c);
  sudoku_rect(sudoku, arr_of_nine, c);
}

void sudoku_horizontal(Sudoku *sudoku, short * arr_of_nine, const Coordinate c) {
  int i, j;
  for (i = 0; i < ARC_SIZE; ++i)
    for (j = 0; j < ARC_SIZE; ++j)
      if (arr_of_nine[i] == sudoku->grid[sudoku_pos(j, c.y)])
        arr_of_nine[i] = 0;
}

void sudoku_vertical(Sudoku *sudoku, short * arr_of_nine, const Coordinate c) {
  int i, j;
  for (i = 0; i < ARC_SIZE; ++i)
    for (j = 0; j < ARC_SIZE; ++j)
      if (arr_of_nine[i] == sudoku->grid[sudoku_pos(c.x, j)])
        arr_of_nine[i] = 0;
}

void sudoku_rect(Sudoku *sudoku, short * arr_of_nine, const Coordinate c) {
  short rx, ry, n, i, j;
  // get y and x of first of the top left of the rect
  rx = (c.x < 3) ? 0 : (c.x < 6) ? 3 : 6;
  ry = (c.y < 3) ? 0 : (c.y < 6) ? 3 : 6;
  // use pos of first element to calc the rect
  i = sudoku_pos(rx, ry);
  for (n = 0; n < ARC_SIZE; ++n)
    for (j = 0; j < 2; ++j )
      if (arr_of_nine[n] == sudoku->grid[i + j * 9] || 
          arr_of_nine[n] == sudoku->grid[i + j * 9 + 1] || 
          arr_of_nine[n] == sudoku->grid[i + j * 9 + 2])
        arr_of_nine[n] = 0;
}

int main (int argc, char const *argv[])
{
  int i;
  Sudoku sudoku;
  
  if (argc == 1) {
    printf("usage: %s <file1> <filen> ...", argv[i]);
    exit(1);
  }
  
  for (i = 1; i < argc; ++i) {
    sudoku_initialize(&sudoku, argv[i]);
    sudoku_solve(&sudoku, 0);
    printf("file %s:\n", argv[i]);
    sudoku_print(&sudoku);
  }
  
  return 0;
}
