#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>

// our game class that handles basically everything
class Screen
{
private:
    int count = 0;

    // true if the game is running, false if not
    bool isRunning;
    // our game window
    SDL_Window *window;
    int BOARD_TILE_WIDTH = 12;
    int BOARD_TILE_HEIGHT = 21;
    int BOARD_PX_HEIGHT = 600;
    int BOARD_PX_WIDTH = 300;
    int BOARD_X_OFFSET = 50;
    int BOARD_Y_OFFSET = 50;
    int mostRecentButton = 0;

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

    void renderBackground();
    void renderBoard();
    void renderCurrentPiece();

    // returns whether the game is currently running or not
    bool running() { return isRunning; }

    // our game renderer
    static SDL_Renderer *renderer;
};

#endif