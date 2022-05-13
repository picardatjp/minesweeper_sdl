#ifndef LABEL_HPP
#define LABEL_HPP

#include "SDL.h"
#include <string>
#include "SDL_ttf.h"

class Label
{
private:
    // the dimensions of the label
    SDL_Rect rect_;
    // the max size the label can be
    int max_w_;
    int max_h_;
    // this stores the loaded font
    SDL_Texture *texture_;
    int cursor_location_;
    // stores the text that is shown in the label
    std::string text_;
    // size of the font
    int font_size_;
    // location of the font, maybe ill make it so you can change this later
    const char *font_path_ = "fonts/bahnschrift.ttf";
    // can the text of the label change
    bool can_modify_ = false;
    // the font
    TTF_Font *font_;
    // color of the text
    SDL_Color color_;
    bool empty_ = false;
    // function pointer to void(void) functions, maybe ill add parameters later if it isn't too hard
    void (*onClickedFunc_)() = nullptr;

public:
    Label();
    ~Label();
    // initialize and set up the label
    void init(SDL_Renderer *renderer, SDL_Rect r, std::string message, SDL_Color color, void (*func)());
    // draw the label to the screen
    void render(SDL_Renderer *r);
    // getters and setters
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
    // this should be called when the button is clicked on
    void onClicked();
    // update the text
    void update(SDL_Renderer *renderer, char c);
    // set modify bool
    void enableModify() { can_modify_ = true; }
    void disableModify() { can_modify_ = false; }
    // loads a font, init helper method
    void loadFont(SDL_Renderer *renderer);
    // reloads the text so the new text can be rendered to the screen
    void reloadText(SDL_Renderer *renderer);
    // change the text of the label
    void setText(SDL_Renderer *renderer, std::string text);
};

#endif