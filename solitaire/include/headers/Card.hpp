#ifndef CARD_HPP
#define CARD_HPP

#include "SDL.h"

class Card
{
private:
    SDL_Rect dRect_;
    SDL_Rect sRect_;
    // 0-A 1-1 .. 11-J 12-Q 13-K
    int number_;
    // 0-club 1-diamond 2-heart - spade
    int suit_;

public:
    SDL_Rect getSrcRect() { return sRect_; }
    SDL_Rect getDestRect() { return dRect_; }
    void setSrcRect(SDL_Rect r) { sRect_ = r; }
    void setDestRect(SDL_Rect r) { dRect_ = r; }
    int getNumber() { return number_; }
    void setNumber(int number) { number_ = number; }
    int getSuit() { return suit_; }
    void setSuit(int suit) { suit_ = suit; }
};

#endif