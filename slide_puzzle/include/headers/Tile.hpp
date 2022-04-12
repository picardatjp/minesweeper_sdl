#ifndef TILE_HPP
#define TILE_HPP

#include "SDL.h"

struct Tile
{
    int id;
    SDL_Rect src;
    SDL_Rect dest;
};

#endif