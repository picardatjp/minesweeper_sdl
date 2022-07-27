#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "SDL.h"
#include <string>

class Button
{
private:
    SDL_Rect _rect;
    SDL_Color _color;
    std::string _text;

public:
    void render();
    void onClick();
};

#endif