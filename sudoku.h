#define ARC_SIZE 9
#define FIELD_SIZE ARC_SIZE * ARC_SIZE
#define TRUE 1
#define FALSE 0

typedef struct {
  short grid[FIELD_SIZE];
  short consts[FIELD_SIZE];
} Sudoku;

typedef struct {
  short x;
  short y;
} Coordinate;

void sudoku_initialize(Sudoku *, const char*);
short sudoku_solve(Sudoku *, const short);
void sudoku_print(Sudoku *);
short sudoku_pos(const short, const short);
Coordinate sudoku_coord(const short);
void sudoku_possible(Sudoku *, short *, const Coordinate c);
void sudoku_horizontal(Sudoku *, short *, const Coordinate c);
void sudoku_vertical(Sudoku *, short *, const Coordinate c);
void sudoku_rect(Sudoku *, short *, const Coordinate c);
  