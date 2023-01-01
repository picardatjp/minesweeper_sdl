#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "SDL.h"

class Button
{
private:
    // has callback function been setup?
    bool cb_set_ = false;
    // pointer to on click func
    void (*on_click_)();
    // button texture
    SDL_Texture *texture_;
    // todo
    bool has_label_ = false;
    int z_index_ = 0;

public:
    // src => rect in texture to use for button
    // dest => rect in application window where the button should be rendered
    SDL_Rect src;
    SDL_Rect dest;
    // set src, dest, texture in one call
    void init(SDL_Rect src_rect, SDL_Rect dest_rect, SDL_Texture *t)
    {
        src = src_rect;
        dest = dest_rect;
        texture_ = t;
    }
    // store func for when button is clicked
    void setOnClick(void (*on_click)())
    {
        on_click_ = on_click;
        cb_set_ = true;
    }
    // callback the stored func
    void onClick()
    {
        if (cb_set_)
            on_click_();
    }
    // set the texture/background image for button
    void setTexture(SDL_Texture *texture) { texture_ = texture; }
    SDL_Texture *getTexture() { return texture_; }
    // render the button to a renderer
    void render(SDL_Renderer *r) { SDL_RenderCopy(r, texture_, &src, &dest); }
    void free() { SDL_DestroyTexture(texture_); }
};

#endif