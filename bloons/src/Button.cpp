#include "Button.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <string>

Button::Button() {}

Button::~Button()
{
    // SDL_DestroyTexture(texture_);
}

void Button::init(SDL_Renderer *renderer, SDL_Rect d, std::string message, SDL_Color button_color, SDL_Color text_color, void (*func)())
{
    dRect_ = d;
    onClickedFunc_ = func;
    color_ = button_color;
    label_->init(renderer, d, message, text_color, nullptr);
    label_->disableModify();
    // SDL_Surface *temp = IMG_Load(path);
    // texture = SDL_CreateTextureFromSurface(r, temp);
    // SDL_FreeSurface(temp);
    if ((d.w - label_->getW()) > 0)
        label_->setX(int((d.w - label_->getW()) / 2) + d.x);
    if ((d.h - label_->getH()) > 0)
        label_->setY(int((d.h - label_->getH()) / 2) + d.y + 2);
    state_ = 0;
}

void Button::update(int x, int y)
{
    // just clicked
    SDL_Rect r1, r2;
    r1.w = r1.h = 1;
    r1.x = x;
    r1.y = y;
    r2 = getRect();
    if (SDL_HasIntersection(&r1, &r2) == SDL_TRUE)
    {
        onClicked();
        // state_ = 0;
    }
}

void Button::render(SDL_Renderer *renderer)
{
    // SDL_RenderCopy(r, texture, &srcRect, &dRect);
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &dRect_);
    label_->render(renderer);
}

void Button::onClicked()
{
    if (onClickedFunc_ != nullptr)
        onClickedFunc_();
}