#ifndef TETRIS_HPP
#define TETRIS_HPP

enum Piece : int
{
    L_shape = 0,
    I_shape = 1,

};

class Tetris
{
private:
    int display_field[200];
    const int BOARD_WIDTH = 12;
    const int BOARD_HEIGHT = 21;
    bool lost = false;
    Piece current_piece;

    // clockwise++, counter clockwise--
    int pieces[7][4][16] = {
        {{0, 0, 0, 0,
          1, 1, 1, 1,
          0, 0, 0, 0,
          0, 0, 0, 0},
         {0, 0, 1, 0,
          0, 0, 1, 0,
          0, 0, 1, 0,
          0, 0, 1, 0},
         {0, 0, 0, 0,
          1, 1, 1, 1,
          0, 0, 0, 0,
          0, 0, 0, 0},
         {0, 0, 1, 0,
          0, 0, 1, 0,
          0, 0, 1, 0,
          0, 0, 1, 0}},
        {{0, 0, 0, 0,
          0, 2, 2, 2,
          0, 0, 2, 0,
          0, 0, 0, 0},
         {0, 0, 2, 0,
          0, 2, 2, 0,
          0, 0, 2, 0,
          0, 0, 0, 0},
         {0, 0, 2, 0,
          0, 2, 2, 2,
          0, 0, 0, 0,
          0, 0, 0, 0},
         {0, 0, 2, 0,
          0, 0, 2, 2,
          0, 0, 2, 0,
          0, 0, 0, 0}},
        {{0, 0, 0, 0,
          0, 3, 3, 0,
          0, 3, 3, 0,
          0, 0, 0, 0},
         {0, 0, 0, 0,
          0, 3, 3, 0,
          0, 3, 3, 0,
          0, 0, 0, 0},
         {0, 0, 0, 0,
          0, 3, 3, 0,
          0, 3, 3, 0,
          0, 0, 0, 0},
         {0, 0, 0, 0,
          0, 3, 3, 0,
          0, 3, 3, 0,
          0, 0, 0, 0}},
        {{0, 0, 0, 0,
          0, 0, 4, 4,
          0, 4, 4, 0,
          0, 0, 0, 0},
         {0, 4, 0, 0,
          0, 4, 4, 0,
          0, 0, 4, 0,
          0, 0, 0, 0},
         {0, 0, 0, 0,
          0, 0, 4, 4,
          0, 4, 4, 0,
          0, 0, 0, 0},
         {0, 4, 0, 0,
          0, 4, 4, 0,
          0, 0, 4, 0,
          0, 0, 0, 0}}};

public:
    bool canRotateCW();
    bool canRotateCCW();
    void rotate();
    void moveLeft();
    void moveRight();
    void moveDown();
    void drop();
    void checkLine();
    void checkLose();
    void clearBoard();
    void newGame();
};

#endif