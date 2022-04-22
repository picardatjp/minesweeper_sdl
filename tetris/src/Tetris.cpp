#include "Tetris.hpp"

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