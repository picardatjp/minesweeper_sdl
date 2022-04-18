#ifndef TILE_HPP
#define TILE_HPP

#include "SDL.h"
// there already is a tile struct
struct tile
{
    unsigned int id;
    SDL_Rect src;
    SDL_Rect dest;
    unsigned int team;
};

#endif