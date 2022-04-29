#include "Game.hpp"
#include "Piece.hpp"
#include <iostream>
#include <vector>

Game::Game() {}
Game::~Game() {}

void Game::setPiece(int index, int piece)
{
    // clear the the location then set it to piece
    board[index] &= empty;
    board[index] |= piece;
}

void Game::movePiece(int from, int to)
{
    // clear the to location, set it to the from piece, then clear the from location
    board[to] &= empty;
    board[to] |= board[from];
    board[from] &= empty;
}

bool Game::isValidMove(int from, int to)
{
    // from piece and to piece
    int fp = getPiece(from);
    int tp = getPiece(to);
    // list of legal moves
    std::vector<int> moves;
    // if the pieces at to and from are on the same team
    if ((fp >> 3) == (tp >> 3))
    {
        return false;
    }
    // check whos turn it is
    if (white_turn && ((fp & white) != white))
    {
        return false;
    }
    if (!white_turn && ((fp & black) != black))
    {
        return false;
    }
    // the rest of this method is really spread out and bad but just for debugging
    // will fix when all movement logic is wroking properly
    else if ((fp & king) == king)
    {
        legalMoves(moves, from);
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] == to)
                return true;
        }
        return false;
    }
    else if ((fp & queen) == queen)
    {
        legalMoves(moves, from);
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] == to)
                return true;
        }
        return false;
    }
    else if ((fp & bishop) == bishop)
    {
        legalMoves(moves, from);
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] == to)
                return true;
        }
        return false;
    }
    else if ((fp & knight) == knight)
    {
        legalMoves(moves, from);
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] == to)
                return true;
        }
        return false;
    }
    else if ((fp & rook) == rook)
    {
        legalMoves(moves, from);
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] == to)
                return true;
        }
        return false;
    }
    else if ((fp & pawn) == pawn)
    {
        legalMoves(moves, from);
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] == to)
                return true;
        }
        return false;
    }
    std::cout << "shouldn't be here. isValidMove()\n";
    return true;
}

// this is a long one
// might break it up even further, like each piece has its own method
void Game::legalMoves(std::vector<int> &moves, int pos)
{
    // the piece at our position
    int piece = getPiece(pos);
    int team = 0;
    // get the team of the piece
    if ((piece & white) > 0)
        team |= white;
    else
        team |= black;
    // if our piece is a king
    if ((piece & king) == king)
    {
        // could be replaced with a loop over possible indexes with bounds checking
        // make sure move is inbounds and not same team
        // upper right
        if (pos % 8 < 7 && int(pos / 8) > 0 && (getPiece(pos - 7) & team) != team)
        {
            moves.push_back(pos - 7);
        }
        // upper left
        if (pos % 8 > 0 && int(pos / 8) > 0 && (getPiece(pos - 9) & team) != team)
        {
            moves.push_back(pos - 9);
        }
        // bottom right
        if (pos % 8 < 7 && int(pos / 8) < 7 && (getPiece(pos + 9) & team) != team)
        {
            moves.push_back(pos + 9);
        }
        // bottom left
        if (pos % 8 > 0 && int(pos / 8) < 7 && (getPiece(pos + 7) & team) != team)
        {
            moves.push_back(pos + 7);
        }
        // right
        if (pos % 8 < 7 && (getPiece(pos + 1) & team) != team)
        {
            moves.push_back(pos + 1);
        }
        // left
        if (pos % 8 > 0 && (getPiece(pos - 1) & team) != team)
        {
            moves.push_back(pos - 1);
        }
        // down
        if (int(pos / 8) < 7 && (getPiece(pos + 8) & team) != team)
        {
            moves.push_back(pos + 8);
        }
        // up
        if (int(pos / 8) > 0 && (getPiece(pos - 8) & team) != team)
        {
            moves.push_back(pos - 8);
        }
    }
    else if ((piece & queen) == queen)
    {
        // eight while loops if not out of bounds and not run into piece
        // just bishop checks and rook checks
        // check up
        int toPos = pos - 8;
        while (toPos >= 0)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos -= 8;
        }
        // check down
        toPos = pos + 8;
        while (toPos <= 63)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos += 8;
        }
        // check right
        toPos = pos + 1;
        while (toPos % 8 > pos % 8)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos++;
        }
        // check left
        toPos = pos - 1;
        while (toPos % 8 < pos % 8)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos--;
        }
        // check up to the right
        toPos = pos - 7;
        while ((toPos % 8 > pos % 8) && toPos >= 0)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos -= 7;
        }
        // check up to the left
        toPos = pos - 9;
        while ((toPos % 8 < pos % 8) && toPos >= 0)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos -= 9;
        }
        // check down to the right
        toPos = pos + 9;
        while ((toPos % 8 > pos % 8) && toPos <= 63)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos += 9;
        }
        // check down to the left
        toPos = pos + 7;
        while ((toPos % 8 < pos % 8) && toPos <= 63)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos += 7;
        }
    }
    else if ((piece & bishop) == bishop)
    {
        // check up to the right
        int toPos = pos - 7;
        // while inbounds
        while ((toPos % 8 > pos % 8) && toPos >= 0)
        {
            // location is empty or piece is enemy piece add move
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            // go the next upper right
            toPos -= 7;
        }
        // check up to the left
        toPos = pos - 9;
        while ((toPos % 8 < pos % 8) && toPos >= 0)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos -= 9;
        }
        // check down to the right
        toPos = pos + 9;
        while ((toPos % 8 > pos % 8) && toPos <= 63)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos += 9;
        }
        // check down to the left
        toPos = pos + 7;
        while ((toPos % 8 < pos % 8) && toPos <= 63)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos += 7;
        }
    }
    else if ((piece & knight) == knight)
    {
        // this could also be an easy loop
        // if location is inbounds and empty or enemy piece
        if (pos % 8 > 0 && int(pos / 8) > 1 && (getPiece(pos - 17) & team) != team)
        {
            moves.push_back(pos - 17);
        }
        if (pos % 8 < 7 && int(pos / 8) > 1 && (getPiece(pos - 15) & team) != team)
        {
            moves.push_back(pos - 15);
        }
        if (pos % 8 > 0 && int(pos / 8) < 6 && (getPiece(pos + 15) & team) != team)
        {
            moves.push_back(pos + 15);
        }
        if (pos % 8 < 7 && int(pos / 8) < 6 && (getPiece(pos + 17) & team) != team)
        {
            moves.push_back(pos + 17);
        }
        if (pos % 8 > 1 && int(pos / 8) > 0 && (getPiece(pos - 10) & team) != team)
        {
            moves.push_back(pos - 10);
        }
        if (pos % 8 > 1 && int(pos / 8) < 7 && (getPiece(pos + 6) & team) != team)
        {
            moves.push_back(pos + 6);
        }
        if (pos % 8 < 6 && int(pos / 8) < 7 && (getPiece(pos + 10) & team) != team)
        {
            moves.push_back(pos + 10);
        }
        if (pos % 8 < 6 && int(pos / 8) > 0 && (getPiece(pos - 6) & team) != team)
        {
            moves.push_back(pos - 6);
        }
    }
    else if ((piece & rook) == rook)
    {
        // check up
        int toPos = pos - 8;
        while (toPos >= 0)
        {
            // if location is empty or enemy piece
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            // go to next up
            toPos -= 8;
        }
        // check down
        toPos = pos + 8;
        // while inbounds
        while (toPos <= 63)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos += 8;
        }
        // check right
        toPos = pos + 1;
        while (toPos % 8 > pos % 8)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos++;
        }
        // check left
        toPos = pos - 1;
        while (toPos % 8 < pos % 8)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                break;
            }
            else
                break;
            toPos--;
        }
    }
    else if ((piece & pawn) == pawn && team == white)
    {
        // up one
        if ((pos - 8 >= 0) && (getPiece(pos - 8) == empty))
        {
            moves.push_back(pos - 8);
        }
        // up two
        if ((int(pos / 8) == 6) && (getPiece(pos - 16) == empty))
        {
            moves.push_back(pos - 16);
        }
        // attack right
        if ((pos - 7 >= 0) && (pos % 8 < 7) && ((getPiece(pos - 7) & black) == black))
        {
            moves.push_back(pos - 7);
        }
        // attack left
        if ((pos - 9 >= 0) && (pos % 8 > 0) && ((getPiece(pos - 9) & black) == black))
        {
            moves.push_back(pos - 9);
        }
    }
    else if ((piece & pawn) == pawn && team == black)
    {
        // down one
        if ((pos + 8 <= 63) && (getPiece(pos + 8) == empty))
        {
            moves.push_back(pos + 8);
        }
        // down two
        if ((int(pos / 8) == 1) && (getPiece(pos + 16) == empty))
        {
            moves.push_back(pos + 16);
        }
        // attack right
        if ((pos + 7 <= 63) && (pos % 8 < 7) && ((getPiece(pos + 7) & white) == white))
        {
            moves.push_back(pos + 7);
        }
        // attack left
        if ((pos + 9 <= 63) && (pos % 8 > 0) && ((getPiece(pos + 9) & white) == white))
        {
            moves.push_back(pos + 9);
        }
    }
}

void Game::clearBoard()
{
    for (int i = 0; i < 64; i++)
    {
        board[i] = empty;
    }
}

void Game::newBoard()
{
    // clear board and set each piece in the starting position
    clearBoard();
    setPiece(0, black | rook);
    setPiece(1, black | knight);
    setPiece(2, black | bishop);
    setPiece(3, black | queen);
    setPiece(4, black | king);
    setPiece(5, black | bishop);
    setPiece(6, black | knight);
    setPiece(7, black | rook);
    setPiece(8, black | pawn);
    setPiece(9, black | pawn);
    setPiece(10, black | pawn);
    setPiece(11, black | pawn);
    setPiece(12, black | pawn);
    setPiece(13, black | pawn);
    setPiece(14, black | pawn);
    setPiece(15, black | pawn);
    setPiece(48, white | pawn);
    setPiece(49, white | pawn);
    setPiece(50, white | pawn);
    setPiece(51, white | pawn);
    setPiece(52, white | pawn);
    setPiece(53, white | pawn);
    setPiece(54, white | pawn);
    setPiece(55, white | pawn);
    setPiece(56, white | rook);
    setPiece(57, white | knight);
    setPiece(58, white | bishop);
    setPiece(59, white | queen);
    setPiece(60, white | king);
    setPiece(61, white | bishop);
    setPiece(62, white | knight);
    setPiece(63, white | rook);
}