#ifndef BLOON_HPP
#define BLOON_HPP

#include "SDL.h"

class Bloon
{
private:
    float x_, y_;
    int pi_ = 0;
    SDL_Texture *tex_;

public:
    SDL_Rect src;
    SDL_Rect dest;
    void setTexture(SDL_Texture *tex) { tex_ = tex; }
    void setX(int x) { dest.x = x; }
    void setY(int y) { dest.y = y; }
    void setSrc(SDL_Rect r)
    {
        src.h = r.h;
        src.w = r.w;
        src.x = r.x;
        src.y = r.y;
    }
    void setDest(SDL_Rect r)
    {
        dest.h = r.h;
        dest.w = r.w;
        dest.x = r.x;
        dest.y = r.y;
    }
    void setPI(int i) { pi_ = i; }
    int getPI() { return pi_; }
    int getHeight() { return src.h; }
    int getWidth() { return src.w; }
    SDL_Texture *getTex() { return tex_; }
};

#endif