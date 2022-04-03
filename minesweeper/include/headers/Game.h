#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <ctime>

// our game class that handles basically everything
class Game
{
private:
    int count = 0;

    // true if the game is running, false if not
    bool isRunning;
    // our game window
    SDL_Window *window;

public:
    // constructor and destructor
    Game();
    ~Game();

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
    bool running() { return isRunning; }

    // our game renderer
    static SDL_Renderer *renderer;
};
void renderBoard();
void newBoard();
void uncoverCells(int x, int y);
bool checkWin();
#endif