#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "SDL.h"
#include <vector>

class Circle
{
private:
    int radius_ = 0;
    SDL_Point center_ = {0, 0};
    SDL_Color fill_color_ = {0, 0, 0, 0};
    SDL_Color border_color_ = {0, 0, 0, 0};
    int border_width_ = 0;
    bool has_border_ = false;
    bool has_fill_ = false;
    // std::vector<SDL_Point> points_;

public:
    void init(int r, SDL_Point c, SDL_Color fc, SDL_Color bc, int bw, bool hb, bool hf)
    {
        radius_ = r;
        center_ = c;
        fill_color_ = fc;
        border_color_ = bc;
        border_width_ = bw;
        has_border_ = hb;
        has_fill_ = hf;
    }
    void setBorderWidth(int bw) { border_width_ = bw; }
    int getBorderWidth() { return border_width_; }
    void setFillColor(SDL_Color c) { fill_color_ = c; }
    SDL_Color getFillColor() { return fill_color_; }
    void setBorderColor(SDL_Color c) { border_color_ = c; }
    SDL_Color getBorderColor() { return border_color_; }
    void setHasFill(bool hf) { has_fill_ = hf; }
    void setHasBorder(bool hb) { has_border_ = hb; }
    void setCenter(SDL_Point c) { center_ = c; }
    SDL_Point getCenter() { return center_; }
    void setRadius(int r) { radius_ = r; }
    int getRadius() { return radius_; }
    void render(SDL_Renderer *r)
    {
        // render the fill and border
        SDL_SetRenderDrawColor(r, fill_color_.r, fill_color_.g, fill_color_.b, fill_color_.a);
        // points_.clear();
        for (int w = 0; w < radius_ * 2; w++)
        {
            for (int h = 0; h < radius_ * 2; h++)
            {
                int dx = radius_ - w;
                int dy = radius_ - h;
                if ((dx * dx + dy * dy) <= (radius_ * radius_))
                {
                    if (has_border_ && ((dx - radius_ < border_width_) && ((dy - radius_) < border_width_)))
                    {
                        SDL_SetRenderDrawColor(r, border_color_.r, border_color_.g, border_color_.b, border_color_.a);
                        // points_.push_back({center_.x + dx, center_.y + dy});
                        SDL_RenderDrawPoint(r, center_.x + dx, center_.y + dy);
                        SDL_SetRenderDrawColor(r, fill_color_.r, fill_color_.g, fill_color_.b, fill_color_.a);
                    }
                    if (has_fill_)
                    {
                        // points_.push_back({center_.x + dx, center_.y + dy});
                        SDL_RenderDrawPoint(r, center_.x + dx, center_.y + dy);
                    }
                }
            }
        }
        // SDL_RenderDrawPoints(r,points_,points_.size());
    }
};

// class Triangle
// {
// private:
//     uint8_t style_flag = 0;
//     SDL_Point points_[3];

// public:
//     void render(SDL_Renderer *r)
//     {
//     }
// };

#endif
