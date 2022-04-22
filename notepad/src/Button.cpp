#include "Button.hpp"
#include "SDL.h"
#include "SDL_image.h"

Button::Button() {}

Button::~Button()
{
    SDL_DestroyTexture(texture);
}

void Button::init(SDL_Rect src, SDL_Rect d, void (*func)(), SDL_Renderer *r, const char *path)
{
    dRect = d;
    srcRect = src;
    onPressedFunc = func;
    // SDL_Surface *temp = IMG_Load(path);
    // texture = SDL_CreateTextureFromSurface(r, temp);
    // SDL_FreeSurface(temp);
    state = 0;
}

void Button::update(int x, int y)
{
    // just clicked
    if (state == 1)
    {
        onPressed();
        state = 0;
    }
}

void Button::render(SDL_Renderer *r)
{
    // SDL_RenderCopy(r, texture, &srcRect, &dRect);
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &dRect);
}

void Button::onPressed()
{
    onPressedFunc();
}