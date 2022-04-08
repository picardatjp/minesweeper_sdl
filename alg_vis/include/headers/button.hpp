#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"
#include "Screen.hpp"

class Button
{
private:
    int x;
    int y;
    int w;
    int h;
    SDL_Texture *texture;

public:
    Button();
    ~Button();
    void init(const char *path);
    void loadTexture();
    void render();
    void freeButtonTexture();
    int getX() { return x; }
    void setX(int mx) { x = mx; }
    int getY() { return y; }
    void setY(int my) { y = my; }
    int getW() { return w; }
    void setW(int mw) { w = mw; }
    int getH() { return h; }
    void setH(int mh) { h = mh; }
};

#endif