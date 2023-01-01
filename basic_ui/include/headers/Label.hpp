#ifndef LABEL_HPP
#define LABEL_HPP

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

class Label
{
private:
    bool initialized = false;
    bool modified_ = false;
    std::string message_;
    SDL_Color bg_color_;
    SDL_Color message_color_;
    SDL_Texture *texture_;
    TTF_Font *font_;
    int font_size_ = 12;

public:
    SDL_Rect dest;
    // renderer,message,font path,font size,message color,dest rect
    void init(SDL_Renderer *r, std::string m, std::string fp, int fs, SDL_Color mc, SDL_Rect d)
    {
        initialized = true;
        dest = d;
        message_ = m;
        message_color_ = mc;
        setFont(r, fp, fs);
    }
    void setFont(SDL_Renderer *r, std::string font_path, int size)
    {
        if (initialized)
        {

            TTF_CloseFont(font_);
            font_size_ = size;
            font_ = TTF_OpenFont(font_path.c_str(), font_size_);
            updateLabel(r);
        }
    }
    void setMessage(SDL_Renderer *r, std::string m)
    {
        if (initialized)
        {
            message_ = m;
            updateLabel(r);
        }
    }
    void updateLabel(SDL_Renderer *r)
    {
        SDL_Surface *ts = TTF_RenderText_Solid(font_, message_.c_str(), message_color_);
        if (ts != NULL)
        {
            SDL_DestroyTexture(texture_);
            texture_ = SDL_CreateTextureFromSurface(r, ts);
            if (texture_ != NULL)
            {
                dest.w = ts->w;
                dest.h = ts->h;
            }
        }
        SDL_FreeSurface(ts);
        modified_ = true;
    }
    void render(SDL_Renderer *r)
    {
        // if (modified_ && initialized)
        // {
        SDL_RenderCopy(r, texture_, NULL, &dest);
        modified_ = false;
        // }
    }
    void free()
    {
        SDL_DestroyTexture(texture_);
        TTF_CloseFont(font_);
    }
};

#endif