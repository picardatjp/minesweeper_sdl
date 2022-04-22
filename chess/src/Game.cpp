#include "Game.hpp"
#include "Piece.hpp"
#include <iostream>
#include <vector>

void Game::setPiece(int index, int piece)
{
    board[index] &= empty;
    board[index] |= piece;
}

void Game::movePiece(int from, int to)
{
    board[to] &= empty;
    board[to] |= board[from];
    board[from] &= empty;
}

bool Game::isValidMove(int from, int to)
{
    bool valid;
    int fp = getPiece(from);
    int tp = getPiece(to);
    std::vector<int> moves;
    // if the pieces at to and from are on the same team
    if ((fp >> 3) == (tp >> 3))
        valid = false;
    // the reset of this method is really spread out and bad but just for debugging
    // will fix when all movement logic is wroking properly
    else if ((fp & king) == king)
    {
        legalMoves(moves, from);
        // std::cout << moves.size() << " ";
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
        // std::cout << moves.size() << " ";
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
        // std::cout << moves.size() << " ";
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
        // std::cout << moves.size() << " ";
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
        // std::cout << moves.size() << " ";
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
        // std::cout << moves.size() << " ";
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] == to)
                return true;
        }
        return false;
    }
    // std::cout << "shouldnt be here: isValidMove()";
    return true;
}

void Game::legalMoves(std::vector<int> &moves, int pos)
{
    int piece = getPiece(pos);
    int team = 0;
    if ((piece & white) > 0)
        team |= white;
    else
        team |= black;
    if ((piece & king) == king)
    {
        // could make this a loop over possible moves and just check if inbounds and not same team, empty is no team so it would work
        // but here is less checks, ex. i don't need to check if pos-7 is out of bounds downward, cause pos is in bounds
        if (pos % 8 < 7 && int(pos / 8) > 0 && (getPiece(pos - 7) & team) != team)
        {
            moves.push_back(pos - 7);
        }
        if (pos % 8 > 0 && int(pos / 8) > 0 && (getPiece(pos - 9) & team) != team)
        {
            moves.push_back(pos - 9);
        }
        if (pos % 8 < 7 && int(pos / 8) < 7 && (getPiece(pos + 9) & team) != team)
        {
            moves.push_back(pos + 9);
        }
        if (pos % 8 > 0 && int(pos / 8) < 7 && (getPiece(pos + 7) & team) != team)
        {
            moves.push_back(pos + 7);
        }
        if (pos % 8 < 7 && (getPiece(pos + 1) & team) != team)
        {
            moves.push_back(pos + 1);
        }
        if (pos % 8 > 0 && (getPiece(pos - 1) & team) != team)
        {
            moves.push_back(pos - 1);
        }
        if (int(pos / 8) < 7 && (getPiece(pos + 8) & team) != team)
        {
            moves.push_back(pos + 8);
        }
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
        while ((toPos % 8 > pos % 8) && toPos >= 0)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
                break;
            }
            else
                break;
            toPos += 7;
        }
    }
    else if ((piece & knight) == knight)
    {
        // could turn this into a loop like i described in the king check

        if (pos % 8 > 0 && int(pos / 8) > 1 && (getPiece(pos - 17) & team) != team)
        {
            moves.push_back(pos - 17);
            // std::cout << (pos - 17) << " ";
        }
        if (pos % 8 < 7 && int(pos / 8) > 1 && (getPiece(pos - 15) & team) != team)
        {
            moves.push_back(pos - 15);
            // std::cout << (pos - 15) << " ";
        }
        if (pos % 8 > 0 && int(pos / 8) < 6 && (getPiece(pos + 15) & team) != team)
        {
            moves.push_back(pos + 15);
            // std::cout << (pos + 15) << " ";
        }
        if (pos % 8 < 7 && int(pos / 8) < 6 && (getPiece(pos + 17) & team) != team)
        {
            moves.push_back(pos + 17);
            // std::cout << (pos + 17) << " ";
        }
        if (pos % 8 > 1 && int(pos / 8) > 0 && (getPiece(pos - 10) & team) != team)
        {
            moves.push_back(pos - 10);
            // std::cout << (pos - 10) << " ";
        }
        if (pos % 8 > 1 && int(pos / 8) < 7 && (getPiece(pos + 6) & team) != team)
        {
            moves.push_back(pos + 6);
            // std::cout << (pos + 6) << " ";
        }
        if (pos % 8 < 6 && int(pos / 8) < 7 && (getPiece(pos + 10) & team) != team)
        {
            moves.push_back(pos + 10);
            // std::cout << (pos + 10) << " ";
        }
        if (pos % 8 < 6 && int(pos / 8) > 0 && (getPiece(pos - 6) & team) != team)
        {
            moves.push_back(pos - 6);
            // std::cout << (pos - 6) << " ";
        }
    }
    else if ((piece & rook) == rook)
    {
        // check up
        int toPos = pos - 8;
        while (toPos >= 0)
        {
            if (getPiece(toPos) == empty)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
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
                // std::cout << " " << toPos;
            }
            else if ((getPiece(toPos) & team) != team)
            {
                moves.push_back(toPos);
                // std::cout << " " << toPos;
                break;
            }
            else
                break;
            toPos--;
        }
    }
    else if ((piece & pawn) == pawn && team == white)
    {
        if ((pos - 8 >= 0) && (getPiece(pos - 8) == empty))
        {
            moves.push_back(pos - 8);
            // std::cout << (pos - 8) << " ";
        }
        if ((int(pos / 8) == 6) && (getPiece(pos - 16) == empty))
        {
            moves.push_back(pos - 16);
            // std::cout << (pos - 16) << " ";
        }
    }
    else if ((piece & pawn) == pawn && team == black)
    {
        if ((pos + 8 <= 63) && (getPiece(pos + 8) == empty))
        {
            moves.push_back(pos + 8);
            // std::cout << (pos + 8) << " ";
        }
        if ((int(pos / 8) == 1) && (getPiece(pos + 16) == empty))
        {
            moves.push_back(pos + 16);
            // std::cout << (pos + 16) << " ";
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