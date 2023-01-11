#ifndef APPLICATION_H
#define APPLICATION_H

#include "SDL.h"
#include <vector>

// our game class that handles basically everything
class Application
{
private:
    // true if the game is running, false if not
    bool is_running_;
    // our game renderer
    SDL_Renderer *renderer_;
    // our game window
    SDL_Window *window_;
    int WINDOW_HEIGHT_ = 600;
    int WINDOW_WIDTH_ = 900;
    const int board_width_ = 7;
    const int board_height_ = 6;
    const int border_buffer_ = 100;
    std::vector<std::pair<int, int>> p1_pieces_;
    std::vector<std::pair<int, int>> p2_pieces_;

public:
    // constructor and destructor
    Application();
    ~Application();

    // init declaration, this starts up the game
    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

    // handles events
    void handleEvents();
    // updates game objects
    void update();
    // refreshes app with updated info
    void render();
    // cleans game memory and stops SDL
    void clean();

    // returns whether the game is currently running or not
    bool running() { return is_running_; }

    void drawGrid();
    void drawPieces();

    int getWinHeight() { return WINDOW_HEIGHT_; }
    int getWinWidth() { return WINDOW_WIDTH_; }
};

#endif