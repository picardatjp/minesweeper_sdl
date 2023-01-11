#ifndef APPLICATION_H
#define APPLICATION_H

#include "SDL.h"
#include "Gate.hpp"
#include <vector>
#include <unordered_map>

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
    std::pair<float, float> render_location_ = std::pair<float, float>(0.0f, 0.0f);
    // std::vector<std::vector<Gate>> gates_;
    // std::unordered_map<Gate, Gate> gatesm_;
    std::vector<Gate> gates_;

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

    // draws the grid in the background
    void drawBackground();
    // draws the gates
    void drawGates();
    void drawWires();

    int getWinHeight() { return WINDOW_HEIGHT_; }
    int getWinWidth() { return WINDOW_WIDTH_; }
};

#endif