#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "SDL.h"
#include <vector>
#include <iostream>

class Sprite
{
private:
    SDL_Texture *tex_ = nullptr;
    SDL_Rect src_ = {0, 0, 0, 0};
    SDL_Rect dest_ = {0, 0, 0, 0};
    int rot_ = 0;
    bool is_flipped_ = false;
    std::vector<SDL_Rect> all_src_;

public:
    Sprite() {}
    ~Sprite()
    {
        // SDL_DestroyTexture(tex_);
        // std::cout << "cleaning...";
    }
    int getX() { return dest_.x; }
    int getY() { return dest_.y; }
    int getW() { return dest_.w; }
    int getH() { return dest_.h; }
    SDL_Texture *getTex() { return tex_; }
    void setX(int x) { dest_.x = x; }
    void setY(int y) { dest_.x = y; }
    void setW(int w) { dest_.x = w; }
    void setH(int h) { dest_.x = h; }
    void setSrc(SDL_Rect src) { src_ = src; }
    void setDest(SDL_Rect dest) { dest_ = dest; }
    void setTex(SDL_Texture *tex) { tex_ = tex; }
    void setRot(int rot) { rot_ = rot; }
    void flip() { is_flipped_ != is_flipped_; }
    void addSrc(SDL_Rect src) { all_src_.push_back(src); }
};

#endif