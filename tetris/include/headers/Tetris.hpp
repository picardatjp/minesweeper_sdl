#ifndef TETRIS_HPP
#define TETRIS_HPP

#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>

struct Piece
{
  int x_offset;
  int y_offset;
  int piece;
  int rotation;
};

class Tetris
{
private:
  int display_field[252] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const int BOARD_WIDTH = 12;
  const int BOARD_HEIGHT = 21;
  int score = 0;
  bool lost = false;
  Piece current_piece;
  Uint32 t;
  int speed = 1000;

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
        0, 0, 0, 0}},
      {{0, 0, 0, 0,
        5, 5, 0, 0,
        0, 5, 5, 0,
        0, 0, 0, 0},
       {0, 0, 5, 0,
        0, 5, 5, 0,
        0, 5, 0, 0,
        0, 0, 0, 0},
       {0, 0, 0, 0,
        5, 5, 0, 0,
        0, 5, 5, 0,
        0, 0, 0, 0},
       {0, 0, 5, 0,
        0, 5, 5, 0,
        0, 5, 0, 0,
        0, 0, 0, 0}},
      {{0, 0, 0, 0,
        0, 6, 6, 6,
        0, 6, 0, 0,
        0, 0, 0, 0},
       {0, 0, 0, 0,
        0, 6, 6, 0,
        0, 0, 6, 0,
        0, 0, 6, 0},
       {0, 0, 0, 0,
        0, 0, 6, 0,
        6, 6, 6, 0,
        0, 0, 0, 0},
       {0, 6, 0, 0,
        0, 6, 0, 0,
        0, 6, 6, 0,
        0, 0, 0, 0}},
      {{0, 0, 0, 0,
        7, 7, 7, 0,
        0, 0, 7, 0,
        0, 0, 0, 0},
       {0, 0, 7, 0,
        0, 0, 7, 0,
        0, 7, 7, 0,
        0, 0, 0, 0},
       {0, 0, 0, 0,
        0, 7, 0, 0,
        0, 7, 7, 7,
        0, 0, 0, 0},
       {0, 0, 0, 0,
        0, 7, 7, 0,
        0, 7, 0, 0,
        0, 7, 0, 0}}};

public:
  Tetris();
  ~Tetris();
  int getPieceElement(int piece, int rotation, int index) { return pieces[piece][rotation][index]; }
  bool canRotateCW();
  bool canRotateCCW();
  void rotate(bool cw);
  void moveLeft();
  void moveRight();
  void moveDown();
  void drop();
  void checkLine();
  void clearBoard();
  void newGame();
  int getDisplayFieldElement(int index) { return display_field[index]; }
  Piece getCurrentPiece() { return current_piece; }
  void setCurrentPiece(Piece p) { current_piece = p; }
  void updateTime();
  bool checkMove();
  void endTurn();
  void newPiece();
  int getScore() { return score; }
  bool getLost() { return lost; }
};

#endif