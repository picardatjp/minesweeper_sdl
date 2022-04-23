#include "Button.hpp"
#include "SDL.h"
#include "SDL_image.h"

Button::Button() {}

Button::~Button()
{
    SDL_DestroyTexture(texture_);
}

void Button::init(SDL_Rect src, SDL_Rect d, void (*func)(), SDL_Renderer *r, const char *path)
{
    dRect_ = d;
    srcRect_ = src;
    onPressedFunc_ = func;
    // SDL_Surface *temp = IMG_Load(path);
    // texture = SDL_CreateTextureFromSurface(r, temp);
    // SDL_FreeSurface(temp);
    state_ = 0;
}

void Button::update(int x, int y)
{
    // just clicked
    if (state_ == 1)
    {
        onPressed();
        state_ = 0;
    }
}

void Button::render(SDL_Renderer *renderer)
{
    // SDL_RenderCopy(r, texture, &srcRect, &dRect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &dRect_);
}

void Button::onPressed()
{
    onPressedFunc_();
}