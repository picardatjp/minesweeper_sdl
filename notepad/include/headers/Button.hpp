#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"

class Button
{
private:
    SDL_Rect srcRect_;
    SDL_Rect dRect_;
    SDL_Texture *texture_;
    void (*onPressedFunc_)();
    // 000 -> idle, 001 -> pressed, 010 -> hover
    unsigned char state_;

public:
    Button();
    ~Button();
    void update(int x, int y);
    void init(SDL_Rect src, SDL_Rect d, void (*func)(), SDL_Renderer *r, const char *path);
    void loadTexture();
    void render(SDL_Renderer *r);
    SDL_Rect getRect() { return dRect_; }
    int getX() { return dRect_.x; }
    void setX(int mx) { dRect_.x = mx; }
    int getY() { return dRect_.y; }
    void setY(int my) { dRect_.y = my; }
    int getW() { return dRect_.w; }
    void setW(int mw) { dRect_.w = mw; }
    int getH() { return dRect_.h; }
    void setH(int mh) { dRect_.h = mh; }
    void onPressed();
    void setState(unsigned char s) { state_ = s; }
    unsigned char getState() { return state_; }
};

#endif