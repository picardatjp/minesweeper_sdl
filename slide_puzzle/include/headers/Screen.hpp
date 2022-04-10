#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SDL.h"
#include "Tile.hpp"
#include <vector>

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
    int win_height = 640;
    int win_width = 640;
    // tile width and height
    int TILE_WIDTH = 10;
    int TILE_HEIGHT = 10;
    int BOARD_HEIGHT = 10;
    int BOARD_WIDTH = 10;
    SDL_Texture *texture;
    SDL_Rect tileset[16];
    Tile tiles[16];
    bool mouseDown = false;
    int initMX = 0;
    int initMY = 0;
    int initTX = 0;
    int initTY = 0;
    int initMX_offset = 0;
    int initMY_offset = 0;
    // values to be sorted
    SDL_Rect board[16];
    // number of integers in values
    int values_size;
    // used to calculate how long it has been since the bar graph was updated
    Uint32 change_time_start = 0;
    Uint32 change_time_end = 0;
    // is values sorted
    bool sorted = false;
    // have the swaps been recorded
    bool sort_recorded = false;
    // stores pairs of indexes that were swapped when the values were sorted
    std::vector<std::pair<int, int>> swaps;
    // stores comparisons, not really using this at the moment
    std::vector<std::pair<int, int>> compares;
    int compares_index = 0;
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
    // updates the bar graph
    void updateGraph();
    // renders the board
    void renderBoard();
    // renders the top part of the screen
    void renderRibbon();

    void newBoard();

    void moveTile();
    void snapTile();

    void swapTile(SDL_Rect &a, SDL_Rect &b);
};

#endif