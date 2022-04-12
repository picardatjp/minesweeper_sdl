#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SDL.h"
#include <stdlib.h>

// our game class that handles basically everything
class Screen
{
private:
    int count = 0;

    // true if the game is running, false if not
    bool isRunning;
    // our game window
    SDL_Window *window;
    // window width and height
    int winHeight = 600;
    int winWidth = 800;

public:
    // constructor and destructor
    Screen();
    ~Screen();

    // init declaration, this starts up the game
    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

    // handles input events
    void handleEvents();
    // updates game objects
    void update();
    // refreshes screen with updated info
    void render();
    // cleans sdl structs memory and stops SDL
    void clean();

    // returns whether the game is currently running or not
    bool running() { return isRunning; }
    // getter and setter for window height and width
    int getWinHeight() { return winHeight; }
    int getWinWidth() { return winWidth; }
    void setWinHeight(int h) { winHeight = h; }
    void setWinWidth(int w) { winWidth = w; }

    void renderScreen();

    // our game renderer
    static SDL_Renderer *renderer;
};
#endif