#include "Tetris.hpp"
#include "SDL.h"
#include <iostream>
#include <random>
// #include <ctime>

Tetris::Tetris() { t = 0; }
Tetris::~Tetris() {}
void Tetris::newGame()
{
    clearBoard();
    newPiece();
    newPiece();
    lost = false;
    score = 0;
}

// this is wrong
void Tetris::clearBoard()
{
    for (int i = 0; i < BOARD_HEIGHT - 1; i++)
    {
        for (int j = 1; j < BOARD_WIDTH - 1; j++)
        {
            display_field[i * BOARD_WIDTH + j] = 0;
        }
    }
}

void Tetris::rotate(bool cw)
{

    if (cw)
    {
        current_piece.rotation = (current_piece.rotation + 1) % 4;
        if (!checkMove())
        {
            if (current_piece.rotation == 0)
                current_piece.rotation = 3;
            else
                current_piece.rotation = (current_piece.rotation - 1) % 4;
        }
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
    if (SDL_GetTicks() - t >= speed)
    {
        moveDown(false);
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
void Tetris::moveDown(bool b)
{
    current_piece.y_offset++;
    if (!checkMove())
    {
        current_piece.y_offset--;
        // spawn new piece and check line
        endTurn();
    }
    else if (b)
        score += 1;
}

void Tetris::drop()
{
    while (checkMove())
    {
        current_piece.y_offset++;
        score += 1;
    }
    score--;
    current_piece.y_offset--;
    endTurn();
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

void Tetris::endTurn()
{
    for (int i = 0; i < 16; i++)
    {
        display_field[(i % 4) + current_piece.x_offset + int(i / 4) * 12 + current_piece.y_offset * 12] = getPieceElement(current_piece.piece, current_piece.rotation, i) | display_field[(i % 4) + current_piece.x_offset + int(i / 4) * 12 + current_piece.y_offset * 12];
    }
    newPiece();
    checkLine();
    if (!checkMove())
    {
        lost = true;
        std::cout << "game over, score = " << score << "!";
    }
}

void Tetris::newPiece()
{
    current_piece = next_piece;
    int x = 0;
    srand(SDL_GetTicks());
    while (x == 0)
    {
        x = (rand() + cnt++) % 8;
    }
    next_piece.x_offset = 4;
    next_piece.y_offset = -1;
    next_piece.rotation = 0;
    next_piece.piece = x - 1;
}

void Tetris::checkLine()
{
    bool x;
    int lines = 0;
    for (int i = 0; i < 20; i++)
    {
        x = true;
        for (int j = 1; j < 11; j++)
        {
            if (getDisplayFieldElement(i * 12 + j) == 0)
                x = false;
        }
        if (x)
        {
            lines++;
            for (int j = (i + 1) * 12; j >= 12; j--)
            {
                display_field[j] = display_field[j - 12];
            }
            for (int j = 1; j < 11; j++)
            {
                display_field[j] = 0;
            }
        }
    }
    if (lines == 1)
        score += 40 * (level + 1);
    if (lines == 2)
        score += 100 * (level + 1);
    if (lines == 3)
        score += 300 * (level + 1);
    if (lines == 4)
        score += 1200 * (level + 1);

    lines_ += lines;
    // std::cout << score << "\n";
}