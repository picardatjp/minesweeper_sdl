#ifndef GAME_HPP
#define GAME_HPP

#include "Piece.hpp"
#include <vector>

class Game
{
private:
    // our 8x8 chess board
    int board[64] = {0};

public:
    // set the pieces to normal beginning state
    void newBoard();
    // set all squares to empty
    void clearBoard();
    // get a piece from the board
    int getPiece(int index) { return board[index]; }
    // put a piece into the board
    void setPiece(int index, int piece);
    // move an existing piece to another square
    void movePiece(int from, int to);
    // checks if the move trying to be made is legal
    bool isValidMove(int from, int to);
    // returns array to legal moves from a given piece
    void legalMoves(std::vector<int> &moves, int pos);
};

#endif