#ifndef GAME_HPP
#define GAME_HPP

#include "Piece.hpp"
#include <vector>

class Game
{
private:
    // our 8x8 chess board
    int board[64] = {0};
    // keeps track of whos turn it is
    bool white_turn = true;
    // bools for check and checkmate
    bool black_in_check = false;
    bool white_in_check = false;
    bool black_checkmate = false;
    bool white_checkmate = false;
    // moved bools for castling, maybe a better way
    bool white_king_moved = false;
    bool black_king_moved = false;
    bool white_right_rook_moved = false;
    bool black_right_rook_moved = false;
    bool white_left_rook_moved = false;
    bool black_left_rook_moved = false;

public:
    Game();
    ~Game();
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
    void setWhiteTurn(bool t) { white_turn = t; }
    bool getWhiteTurn() { return white_turn; }
};

#endif