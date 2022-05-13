#include "Tetris.hpp"

Tetris::Tetris() {}
Tetris::~Tetris() {}
void Tetris::newGame()
{
    clearBoard();
}
void Tetris::clearBoard()
{
    for (int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
    {
        display_field[i] = 0;
    }
}

void Tetris::rotate(bool cw)
{

    if (cw)
        current_piece.rotation = (current_piece.rotation + 1) % 4;
    else
    {
        if (current_piece.rotation > 0)
            current_piece.rotation = (current_piece.rotation - 1) % 4;
        else
            current_piece.rotation = 3;
    }
}

void Tetris::moveLeft()
{
    current_piece.x_offset--;
}
void Tetris::moveRight()
{
    current_piece.x_offset++;
}
void Tetris::moveDown()
{
    current_piece.y_offset++;
}