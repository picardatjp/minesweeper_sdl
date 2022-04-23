#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include <string>
#include "SDL.h"
#include "Label.hpp"
#include <memory>

class Textbox
{
private:
    std::unique_ptr<Label> label_ = std::make_unique<Label>();
    SDL_Rect rect_;
    // SDL_Texture *texture_;
    // function pointer to void(void) functions, maybe ill add parameters later if it isn't too hard
    void (*onPressedFunc_)() = nullptr;

public:
    Textbox();
    ~Textbox();
    void init(SDL_Renderer *renderer, SDL_Rect r, void (*func)());
    void render(SDL_Renderer *r);
    SDL_Rect getRect() { return rect_; }
    int getX() { return rect_.x; }
    void setX(int mx) { rect_.x = mx; }
    int getY() { return rect_.y; }
    void setY(int my) { rect_.y = my; }
    int getW() { return rect_.w; }
    void setW(int mw) { rect_.w = mw; }
    int getH() { return rect_.h; }
    void setH(int mh) { rect_.h = mh; }
    void onPressed();
    void update(SDL_Renderer *renderer, char c);
};

#endif