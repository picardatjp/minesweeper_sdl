#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SDL.h"

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
    // window dimensions
    int win_height = 512;
    int win_width = 512;
    // tile width and height
    int TILE_WIDTH = 64;
    int TILE_HEIGHT = 64;
    int BOARD_HEIGHT = 8;
    int BOARD_WIDTH = 8;
    // chess pieces texture
    SDL_Texture *texture;
    // source rectangles for pieces from the texture
    SDL_Rect tile_src[12];
    // index of the piece we are holding
    int selected_tile = -1;
    // position of the piece we are holding
    SDL_Rect selected_rect;
    // is mouse being pressed bool
    bool mouse_down = false;
    // initial mouse click position
    int initMX = 0;
    int initMY = 0;
    // offset from top left of tile clicked
    int initMX_offset = 0;
    int initMY_offset = 0;
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
    // renders the board
    void renderBoard();
    // renders the top part of the screen
    void renderRibbon();
    // render the piece we are holding
    void renderSelectedPiece();
    // make the piece we are holding follow the cursor
    void moveTile();
};

#endif