#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"

class Button
{
private:
    bool cb_set_ = false;
    void (*on_click_)();
    SDL_Texture *texture_;
    // todo
    bool has_label_ = false;
    int z_index_ = 0;

public:
    SDL_Rect src;
    SDL_Rect dest;
    void setOnClick(void (*on_click)())
    {
        on_click_ = on_click;
        cb_set_ = true;
    }
    void onClick()
    {
        if (cb_set_)
            on_click_();
    }
    void setTexture(SDL_Texture *texture) { texture_ = texture; }
    SDL_Texture *getTexture() { return texture_; }
    void render(SDL_Renderer *r) { SDL_RenderCopy(r, texture_, &src, &dest); }
};

#endif