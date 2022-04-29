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
    int pic_selected = 0;
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
    // int BOARD_HEIGHT = 4;
    // int BOARD_WIDTH = 4;
    SDL_Texture *texture;
    Tile tiles[16];
    bool mouseDown = false;
    int initMX = 0;
    int initMY = 0;
    int initMX_offset = 0;
    int initMY_offset = 0;
    // is values sorted
    bool sorted = false;
    // height of the grey ribbon at the top of the screen
    int ribbon_offset = 64;
    int current_screen = 0;
    SDL_Texture *buttons_texture;
    SDL_Rect buttons_src;
    SDL_Rect buttons_dest;
    SDL_Rect button_highlight_dest;
    SDL_Rect button_highlight_src;
    SDL_Texture *play_button_texture;
    SDL_Rect play_button_src;
    SDL_Rect play_button_dest;
    SDL_Texture *bg_texture;

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
    // renders start menu
    void renderMenu();
    void newBoard();

    void moveTile();
    void snapTile();
    void checkWin();
    bool isSolvable();

    void loadTexture(int pic);
    void playClicked();
};

#endif