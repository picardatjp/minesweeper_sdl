#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SDL.h"
#include <vector>

struct point
{
    float x;
    float y;
};

struct spline
{
    std::vector<point> points;
    point getSplinePoint(float t)
    {
        int p0, p1, p2, p3;
        p1 = (int)t + 1;
        p2 = p1 + 1;
        p3 = p2 + 1;
        p0 = p1 - 1;
        t = t - (int)t;
        float tt = t * t;
        float ttt = tt * t;
        float q1 = -ttt + 2.0f * tt - t;
        float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
        float q3 = -3.0f * ttt + 4.0f * tt + t;
        float q4 = ttt - tt;
        float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
        float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);
        return {tx, ty};
    }
};

// our app class that handles basically everything
class Application
{
private:
    // true if the game is running, false if not
    bool is_running_;
    // our game window
    SDL_Window *window_;
    // window width and height
    int win_height_ = 480;
    int win_width_ = 640;
    bool mouse_down_ = false;

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
    float lerp(float a, float b, float t);
    void setPixel(int x, int y, SDL_Surface *s);
};

#endif