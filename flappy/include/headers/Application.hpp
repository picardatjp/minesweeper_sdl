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

    SDL_Rect bird_rect_;
    float bird_y_;
    float bird_x_;
    float gravity_;
    std::vector<SDL_Rect> pillar_rects_;
    bool space_down_ = false;
    bool jump_reset_ = true;
    float y_v_ = 0;
    int pillar_width_ = 100;
    int pillar_gap_ = 200;
    SDL_Texture *pillar_tex_;
    SDL_Texture *bird_tex_;
    SDL_Texture *gameover_tex_;
    SDL_Texture *numbers_tex_;
    bool gameover_ = false;
    int scores_[3] = {0, 0, 0};
    int score_ = 0;
    bool gamestart_ = false;
    Uint32 d_start_ = 0;

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

    void checkPillarBounds();
    void randomizePillarY(int);
    void checkPillarCollision();
    void getScores();
    void displayEndScreen();
    void displayScore();
};

#endif