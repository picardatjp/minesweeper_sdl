#include "Textbox.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>

Textbox::Textbox() {}
Textbox::~Textbox()
{
    // SDL_DestroyTexture(texture_);
}
void Textbox::init(SDL_Renderer *renderer, SDL_Rect r, void (*func)())
{
    rect_ = r;
    onPressedFunc_ = func;
    SDL_Color c;
    c.r = c.g = c.b = 0;
    c.a = 255;
    label_->init(renderer, r, "", c, nullptr);
}

void Textbox::onPressed()
{
    if (!onPressedFunc_)
        onPressedFunc_();
}

void Textbox::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect_);
    label_->render(renderer);
}

void Textbox::update(SDL_Renderer *renderer, char c)
{
    label_->update(renderer, c);
}