#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"

class Button
{
private:
    SDL_Rect srcRect;
    SDL_Rect dRect;
    SDL_Texture *texture;
    void (*onPressedFunc)();
    // 000 -> idle, 001 -> pressed, 010 -> hover
    unsigned char state;

public:
    Button();
    ~Button();
    void update(int x, int y);
    void init(SDL_Rect src, SDL_Rect d, void (*func)(), SDL_Renderer *r, const char *path);
    void loadTexture();
    void render(SDL_Renderer *r);
    SDL_Rect getRect() { return dRect; }
    int getX() { return dRect.x; }
    void setX(int mx) { dRect.x = mx; }
    int getY() { return dRect.y; }
    void setY(int my) { dRect.y = my; }
    int getW() { return dRect.w; }
    void setW(int mw) { dRect.w = mw; }
    int getH() { return dRect.h; }
    void setH(int mh) { dRect.h = mh; }
    void onPressed();
    void setState(unsigned char s) { state = s; }
    unsigned char getState() { return state; }
};

#endif