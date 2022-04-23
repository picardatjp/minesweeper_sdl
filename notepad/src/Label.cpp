#include "Label.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>

Label::Label() {}
Label::~Label()
{
    SDL_DestroyTexture(texture_);
}
void Label::init(SDL_Renderer *renderer, SDL_Rect r, std::string message, SDL_Color color, void (*func)())
{
    rect_ = r;
    max_w_ = r.w;
    max_h_ = r.h;
    color_ = color;
    onPressedFunc_ = func;
    font_size_ = 24;
    text_ = message;
    loadFont(renderer);
}

void Label::onPressed()
{
    if (!onPressedFunc_)
        onPressedFunc_();
}

void Label::render(SDL_Renderer *renderer)
{
    if (!empty_)
    {
        SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
    }
}

void Label::loadFont(SDL_Renderer *renderer)
{
    font_ = TTF_OpenFont(font_path_, font_size_);
    SDL_Surface *temp;
    if (!font_)
        std::cout << "Failed to load font.\n"
                  << std::endl;
    if (text_.size() < 1)
    {
        text_ = " ";
        temp = TTF_RenderText_Solid_Wrapped(font_, text_.c_str(), color_, max_w_);
        text_ = "";
    }
    else
        temp = TTF_RenderText_Solid_Wrapped(font_, text_.c_str(), color_, max_w_);
    if (!temp)
        std::cout << "Failed to create surface\n"
                  << std::endl;
    texture_ = SDL_CreateTextureFromSurface(renderer, temp);
    if (!texture_)
        std::cout << "failed to create texture\n";
    if (temp->w > max_w_)
        rect_.w = max_w_;
    else
        rect_.w = temp->w;
    rect_.h = temp->h;
    SDL_FreeSurface(temp);
}

void Label::reloadText(SDL_Renderer *renderer)
{
    SDL_DestroyTexture(texture_);
    SDL_Surface *temp = TTF_RenderText_Solid_Wrapped(font_, text_.c_str(), color_, max_w_);
    if (!temp)
        std::cout << "Failed to create surface\n"
                  << std::endl;
    texture_ = SDL_CreateTextureFromSurface(renderer, temp);
    if (!texture_)
        std::cout << "failed to create texture\n";
    rect_.w = temp->w;
    rect_.h = temp->h;
    SDL_FreeSurface(temp);
}

void Label::update(SDL_Renderer *renderer, char c)
{

    if (c == char(8))
    {
        if (text_.size() <= 1)
        {
            text_ = "";
            // reloadText(renderer);
            empty_ = true;
        }
        else
        {
            text_ = text_.substr(0, text_.size() - 1);
            reloadText(renderer);
        }
    }
    else if (c == char(32))
    {
        text_ += " ";
        reloadText(renderer);
        empty_ = false;
    }
    else
    {
        text_ += c;
        reloadText(renderer);
        empty_ = false;
    }
}