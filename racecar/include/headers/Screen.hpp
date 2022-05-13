#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SDL.h"
// #include <algorithm>

// our screen class that handles basically everything
class Screen
{
private:
    // true if the game is running, false if not
    bool is_running_;
    // our game window
    SDL_Window *window;
    SDL_Renderer *renderer;
    // window width and height
    int win_height_ = 800;
    int win_width_ = 1000;
    // height of the grey ribbon at the top of the screen
    int ribbon_offset = 64;
    bool mouse_down_ = false;
    bool mouse_release_ = false;
    bool left_down_ = false;
    bool right_down_ = false;
    bool up_down_ = false;
    bool down_down_ = false;
    const int SPEED_ = 5;
    SDL_Rect car_dest_;
    float car_x_ = 0.0f;
    float car_y_ = 0.0f;
    float car_x_speed_ = 0.0f;
    float car_y_speed_ = 0.0f;
    float min_speed_ = float(-SPEED_);
    float max_speed_ = float(SPEED_);
    int dir_ = 0;
    int last_dir_ = 0;

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
    bool running() { return is_running_; }
    // getter and setter for window height and width
    int getWinHeight() { return win_height_; }
    int getWinWidth() { return win_width_; }
    void setWinHeight(int h) { win_height_ = h; }
    void setWinWidth(int w) { win_width_ = w; }
    void moveCar();
    void renderCar();
};

#endif