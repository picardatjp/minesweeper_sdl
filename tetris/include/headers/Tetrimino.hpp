#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "Game.h"

class Tetrimino
{
public:
    Tetrimino();
    Tetrimino(int t);
    ~Tetrimino();
    void moveLeft();
    void moveRight();
    void rotateCCW();
    void rotateCW();
    int getType() { return type; }
    int getX() { return x; }
    int getY() { return y; }
    void setX(int mx) { x = mx; }
    void setY(int my) { y = my; }

private:
    int type;
    int x;
    int y;
};

#endif