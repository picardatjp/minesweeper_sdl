#ifndef APPLICATION_H
#define APPLICATION_H

#include "SDL.h"
#include <vector>
#include "Spline.hpp"
#include "Bloon.hpp"

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
    spline s_;
    std::vector<point> line;
    int count;
    int bloon_height, bloon_width;
    std::vector<Bloon> bloons;
    SDL_Texture *bg;

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

    void createSpline(std::vector<point> &line);
    float lerp(float a, float b, float t);
};

#endif