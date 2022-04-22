#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "SDL.h"

class Textbox
{
private:
    SDL_Rect rect;
    int cursor_location;
    char *text;
    int font_size;
    int font; // dont know what type a font would be
    // function pointer to void(void) functions, maybe ill add parameters later if it isn't too hard
    void (*onPressedFunc)();

public:
    Textbox();
    ~Textbox();
    void init(SDL_Rect r, void (*func)());
    void render(SDL_Renderer *r);
    SDL_Rect getRect() { return rect; }
    int getX() { return rect.x; }
    void setX(int mx) { rect.x = mx; }
    int getY() { return rect.y; }
    void setY(int my) { rect.y = my; }
    int getW() { return rect.w; }
    void setW(int mw) { rect.w = mw; }
    int getH() { return rect.h; }
    void setH(int mh) { rect.h = mh; }
    void onPressed();
    void update();
};

#endif