#include "Textbox.hpp"
#include "SDL.h"

Textbox::Textbox() {}
Textbox::~Textbox() {}
void Textbox::init(SDL_Rect r, void (*func)())
{
    rect = r;
    onPressedFunc = func;
}

void Textbox::onPressed()
{
    onPressedFunc();
}

void Textbox::render(SDL_Renderer *r)
{
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderFillRect(r, &rect);
}