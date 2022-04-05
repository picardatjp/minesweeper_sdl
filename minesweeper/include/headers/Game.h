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
    // window width and height
    int winHeight = 664;
    int winWidth = 600;

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
    // getter and setter for window height and width
    int getWinHeight() { return winHeight; }
    int getWinWidth() { return winWidth; }
    void setWinHeight(int h) { winHeight = h; }
    void setWinWidth(int w) { winWidth = w; }

    // our game renderer
    static SDL_Renderer *renderer;
};

// renders the minewsweeper board
void renderBoard();
// resets the board and changes bomb locations
void newBoard();
// uncovers cells whenever the user clicks on a cell
void uncoverCells(int x, int y);
// checks to see if the user won
bool checkWin();
// allows for manual placement of bombs
void manualBomb(int bombs[2][8]);
// updates and renders timer to the screen
void renderTimer();
// get and reset timer seconds
int getTimeElapsed();
void resetTimer();
#endif