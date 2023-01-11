#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"
#include "SDL_image.h"
#include "Label.hpp"
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
    int BOARD_X_OFFSET = 310;
    int BOARD_Y_OFFSET = 163;
    int NEXT_PIECE_X_OFFSET = 552;
    int NEXT_PIECE_Y_OFFSET = 410;
    int mostRecentButton = 0;
    int key_presses = 0;
    int current_screen = 0;
    int tile_size = 28;
    Label score_lbl_;
    int score_lbl_val = 0;
    Label level_lbl_;
    int level_lbl_val = 0;
    Label top_score_lbl_;
    Label stat_lbls[7];
    int stat_lbl_vals[7];
    Label lines_lbl_;
    int lines_lbl_val = 0;

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

    void setScoreLabel(int s) { score_lbl_.setMessage(renderer, std::to_string(s)); }
    void renderLabels();
    void renderBackground();
    void renderBoard();
    void renderCurrentPiece();
    void initLabels();

    // returns whether the game is currently running or not
    bool running() { return isRunning; }

    // our game renderer
    static SDL_Renderer *renderer;
};

#endif