#ifndef PIECE_HPP
#define PIECE_HPP

// a bunch of piece flags
enum Piece : int
{
    empty = 0,
    pawn = 1,
    rook = 2,
    knight = 3,
    bishop = 4,
    queen = 5,
    king = 6,
    white = 8,
    black = 16,
};

#endif