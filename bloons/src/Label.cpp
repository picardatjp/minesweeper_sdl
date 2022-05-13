#include "Label.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>

Label::Label() {}
Label::~Label()
{
    // free our texture
    SDL_DestroyTexture(texture_);
}
void Label::init(SDL_Renderer *renderer, SDL_Rect r, std::string message, SDL_Color color, void (*func)())
{
    // get the dimensions and set our max to the original size
    rect_ = r;
    max_w_ = r.w;
    max_h_ = r.h;
    color_ = color;
    onClickedFunc_ = func;
    font_size_ = 24;
    text_ = message;
    // finally load the font and we are all good
    loadFont(renderer);
    // should probably init can_modify_ in here at some point
}

void Label::onClicked()
{
    // if onClickedFunc is pointing to a function then run it
    if (onClickedFunc_ != nullptr)
        onClickedFunc_();
}

void Label::render(SDL_Renderer *renderer)
{
    // if the label isn't empty we want to display it
    if (!empty_)
    {
        SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
    }
}

void Label::loadFont(SDL_Renderer *renderer)
{
    // open the font
    font_ = TTF_OpenFont(font_path_, font_size_);
    SDL_Surface *temp;
    if (!font_)
        std::cout << "Failed to load font.\n"
                  << std::endl;
    // if our text is "" (empty) then it wont work, needs to be not empty string
    if (text_.size() < 1)
    {
        // so we put a space in place of the empty string
        text_ = " ";
        temp = TTF_RenderText_Solid_Wrapped(font_, text_.c_str(), color_, max_w_);
        // and then go back setting empty to true
        text_ = "";
        empty_ = true;
    }
    else
        // otherwise just make our text
        temp = TTF_RenderText_Solid_Wrapped(font_, text_.c_str(), color_, max_w_);
    if (!temp)
        std::cout << "Failed to create surface\n"
                  << std::endl;
    // save the texture of text we created
    texture_ = SDL_CreateTextureFromSurface(renderer, temp);
    if (!texture_)
        std::cout << "failed to create texture\n";
    // if our width is greater than the orignal dimensions we were given then we want to restrict the width
    //  so that we can have the text wrap, not really necessary for height
    //  this step might actually not be necessary im not sure, haven't not used it
    if (temp->w > max_w_)
        rect_.w = max_w_;
    else
        rect_.w = temp->w;
    rect_.h = temp->h;
    // free the unneeded temp surface
    SDL_FreeSurface(temp);
}

void Label::reloadText(SDL_Renderer *renderer)
{
    // destroy the texture that was stored in texture_ and we replace it
    SDL_DestroyTexture(texture_);
    // same process as load font accept no empty checks
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
    // if we are allowed to modify the label
    if (can_modify_)
    {
        // backspace
        if (c == char(8))
        {
            if (text_.size() <= 1)
            {
                text_ = "";
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
}

void Label::setText(SDL_Renderer *renderer, std::string text)
{
    if (can_modify_)
    {
        text_ = text;
        if (text.size() < 1)
            empty_ = true;
        else
            reloadText(renderer);
    }
}