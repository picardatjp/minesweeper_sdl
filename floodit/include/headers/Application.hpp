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
    // our game window
    SDL_Window *window_;
    int WINDOW_HEIGHT_ = 700;
    int WINDOW_WIDTH_ = 600;

    int GAME_BOARD_DIM_ = 600;
    std::vector<int> board_;
    int board_num_cells_;
    int curr_color_;
    int picked_color_;
    int score_;
    float go_r_, go_b_, go_g_;
    float go_r_v_, go_b_v_, go_g_v_;
    SDL_Texture *score_tex_;
    SDL_Texture *ng_tex_;
    SDL_Texture *bs_tex_;

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

    int getWinHeight() { return WINDOW_HEIGHT_; }
    int getWinWidth() { return WINDOW_WIDTH_; }
    void floodFill(int x, int y);
    bool checkGameEnd();
    void randomizeBoard();
    void renderGameBoard();
    void renderUI();
    // our game renderer
    SDL_Renderer *renderer = nullptr;
};

#endif