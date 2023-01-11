#ifndef APPLICATION_H
#define APPLICATION_H

#include "SDL.h"
#include <cmath>

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
    int sx_ = 0;
    float freq1_ = 1.5f;
    float freq2_ = 8.0f;
    float amp_ = 100.0f;
    float amp2_ = 100.f;
    float freq1i_ = 0.02f;
    float freq2i_ = 0.2f;
    float amp1i_ = 0.1f;

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
    SDL_Point stringSineY(float y, float a);
    void drawString();

    int getWinHeight() { return WINDOW_HEIGHT_; }
    int getWinWidth() { return WINDOW_WIDTH_; }
};

#endif