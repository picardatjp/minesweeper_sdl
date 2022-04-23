#ifndef LABEL_HPP
#define LABEL_HPP

#include "SDL.h"
#include <string>
#include "SDL_ttf.h"

class Label
{
private:
    SDL_Rect rect_;
    int max_w_;
    int max_h_;
    SDL_Texture *texture_;
    int cursor_location_;
    std::string text_;
    int font_size_;
    const char *font_path_ = "fonts/bahnschrift.ttf";
    bool can_modify_ = false;
    TTF_Font *font_;
    SDL_Color color_;
    bool empty_ = false;
    // function pointer to void(void) functions, maybe ill add parameters later if it isn't too hard
    void (*onClickedFunc_)() = nullptr;

public:
    Label();
    ~Label();
    void init(SDL_Renderer *renderer, SDL_Rect r, std::string message, SDL_Color color, void (*func)());
    void render(SDL_Renderer *r);
    SDL_Rect getRect() { return rect_; }
    int getX() { return rect_.x; }
    void setX(int mx) { rect_.x = mx; }
    int getY() { return rect_.y; }
    void setY(int my) { rect_.y = my; }
    int getW() { return rect_.w; }
    void setW(int mw) { rect_.w = mw; }
    int getH() { return rect_.h; }
    void setH(int mh) { rect_.h = mh; }
    // wont work, need to recreate font with TTF_OpenFont() to take effect
    void setFontSize(int size) { font_size_ = size; }
    void onClicked();
    void update(SDL_Renderer *renderer, char c);
    void enableModify() { can_modify_ = true; }
    void disableModify() { can_modify_ = false; }
    void loadFont(SDL_Renderer *renderer);
    void reloadText(SDL_Renderer *renderer);
    void setText(SDL_Renderer *renderer, std::string text);
};

#endif