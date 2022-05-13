#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <vector>
#include "SDL.h"
#include "Button.hpp"

class Menu
{
private:
    std::vector<std::unique_ptr<Button>> buttons_;
    SDL_Rect rect_;
    SDL_Color color_;

public:
    Menu();
    ~Menu();
    void update(int x, int y);
    void init(int screen_width, int menu_height, SDL_Color color);
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
    void addButton(SDL_Renderer *renderer, int width, std::string message, SDL_Color button_color, SDL_Color text_color, void (*func)());
};

#endif