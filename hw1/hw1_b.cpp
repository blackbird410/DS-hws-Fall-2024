#include <iostream>

#define SIZE 4
#define SPACE 0
#define SOLVED 1

using namespace std;

class SudokuBoard {
public:
  SudokuBoard() : state(0) {};
  SudokuBoard(int (&values)[SIZE*SIZE]) {
    int i, j;
    for (i = 0; i < SIZE * SIZE; ++i)
      setCell(i / SIZE, i % SIZE, values[i]);
    setState(isSolved());
  };
  ~SudokuBoard() {};

  int isValidRowCol(int v) const {
    return (v < SIZE || v >= 0);
  };

  int isValid(int value) const {
    return (value <= SIZE && value >= 0);
  };

  void setCell(int row, int col, int value) {
    if (!isValidRowCol(row) || !isValidRowCol(col) || !isValid(value)) {
      cout << "Invalid value" << endl;
      return;
    };
    board[row][col] = value;
  }
  
  int getCell(int row, int col) const {
    return (isValidRowCol(row) && isValidRowCol(col)) ? board[row][col] : SPACE;
  }

  void setState(int value) { state = value; }
  int getState() const { return state; };
  

  void fillBoard() {
    int i, j, temp;
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
        temp = SIZE + 1;
        while (!isValid(temp)) {
          cin >> temp;
        }
        board[i][j] = temp;
      }
    }
  };

  int isSolved() const {
    int i, j;
    for (i = 0; i < SIZE; ++i)
      for (j = 0; j < SIZE; ++j)
        if (board[i][j] == SPACE) 
          return !SOLVED;
    return SOLVED;
  };

  int rowContains(int row, int value) {
    for (int j = 0; j < SIZE; ++j)
      if (board[row][j] == value) return 1;
    return 0;
  };

  int colContains(int col, int value) {
    for (int i = 0; i < SIZE; ++i)
      if (board[i][col] == value) return 1;
    return 0;
  }; 

  int boxContains(int row, int col, int value) {
    int startRow = (row / 2) * 2;
    int startCol = (col / 2) * 2;
    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 2; ++j)
        if (board[startRow + i][startCol + j] == value) return 1;
    return 0;
  }

  friend ostream& operator<<(ostream& out, SudokuBoard& s) {
    int i, j;
    
    for (i = 0; i < SIZE; ++i) {
      for (j = 0; j < SIZE; ++j) {
        out << s.board[i][j];
        if (j < SIZE - 1) cout << " ";
      }
      if (i < SIZE - 1) cout << endl;
    }

    return out;
  };

private:
  int board[SIZE][SIZE];
  int state;
};

int solveSudoku(SudokuBoard *b) {
  if (b->isSolved()) {
    b->setState(SOLVED);
    return 1;
  }
  
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      if (b->getCell(i, j) == SPACE) {
        for (int k = 1; k <= SIZE; ++k) {
          if (!b->rowContains(i, k) && !b->colContains(j, k) && !b->boxContains(i, j, k)) {
            b->setCell(i, j, k);
            cout << *b << endl;
            if (solveSudoku(b)) return 1;
            b->setCell(i, j, SPACE);
          }
        }
        return 0;
      }
    }
  }
  return 0;
}

int main() {
  SudokuBoard b;
  b.fillBoard();

  solveSudoku(&b);

  return 0;
}

