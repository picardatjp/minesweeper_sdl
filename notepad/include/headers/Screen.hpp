#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SDL.h"
#include <vector>
#include <random>
// #include <algorithm>

// our screen class that handles basically everything
class Screen
{
private:
    int count = 0;

    // true if the game is running, false if not
    bool is_running;
    // our game window
    SDL_Window *window;
    SDL_Renderer *renderer;
    // window width and height
    int win_height = 480;
    int win_width = 640;
    // height of the grey ribbon at the top of the screen
    int ribbon_offset = 64;

public:
    // constructor and destructor
    Screen();
    ~Screen();

    // init declaration, this starts up the game
    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

    // handles events
    void handleEvents();
    // updates game objects
    void update();
    // refreshes screen with updated info
    void render();
    // cleans game memory and stops SDL
    void clean();

    // returns whether the game is currently running or not
    bool running() { return is_running; }
    // getter and setter for window height and width
    int getWinHeight() { return win_height; }
    int getWinWidth() { return win_width; }
    void setWinHeight(int h) { win_height = h; }
    void setWinWidth(int w) { win_width = w; }
    // updates text in the textbox
    void updateText();
    // renders the top part of the screen
    void renderRibbon();
    // renders the buttons
    void renderButtions();
};

#endif