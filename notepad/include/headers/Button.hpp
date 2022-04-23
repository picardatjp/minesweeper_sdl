#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"
#include "Label.hpp"
#include <memory>
#include <string>
class Button
{
private:
    std::unique_ptr<Label> label_ = std::make_unique<Label>();
    // SDL_Rect srcRect_;
    SDL_Rect dRect_;
    // SDL_Texture *texture_;
    void (*onClickedFunc_)() = nullptr;
    // 000 -> idle, 001 -> pressed, 010 -> hover
    unsigned char state_;
    SDL_Color color_;

public:
    Button();
    ~Button();
    void update(int x, int y);
    void init(SDL_Renderer *renderer, SDL_Rect d, std::string message, SDL_Color button_color, SDL_Color text_color, void (*func)());
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
    void onClicked();
    void setState(unsigned char s) { state_ = s; }
    unsigned char getState() { return state_; }
};

#endif