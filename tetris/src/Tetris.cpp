#include "Tetris.hpp"
#include "SDL.h"

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
    {
        current_piece.rotation = (current_piece.rotation + 1) % 4;
        if (!checkMove())
            current_piece.rotation = (current_piece.rotation - 1) % 4;
    }
    else
    {
        if (current_piece.rotation > 0)
        {
            current_piece.rotation = (current_piece.rotation - 1) % 4;
            if (!checkMove())
                current_piece.rotation = (current_piece.rotation + 1) % 4;
        }
        else
        {
            current_piece.rotation = 3;
            if (!checkMove())
                current_piece.rotation = 0;
        }
    }
}

void Tetris::updateTime()
{
    t = SDL_GetTicks();
    if (SDL_GetTicks() - t >= speed)
    {
        moveDown();
        t = SDL_GetTicks();
    }
}

void Tetris::moveLeft()
{
    current_piece.x_offset--;
    if (!checkMove())
        current_piece.x_offset++;
}
void Tetris::moveRight()
{
    current_piece.x_offset++;
    if (!checkMove())
        current_piece.x_offset--;
}
void Tetris::moveDown()
{
    current_piece.y_offset++;
    if (!checkMove())
        current_piece.y_offset--;
}

bool Tetris::checkMove()
{
    for (int i = 0; i < 16; i++)
    {
        if (display_field[(i % 4) + current_piece.x_offset + int(i / 4) * 12 + current_piece.y_offset * 12] > 0 && getPieceElement(current_piece.piece, current_piece.rotation, i) > 0)
        {
            return false;
        }
    }
    return true;
}