#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <ctime>

// our screen class that handles basically everything
class Screen
{
private:
    int count = 0;

    // true if the game is running, false if not
    bool isRunning;
    // our game window
    SDL_Window *window;
    // window width and height
    int winHeight = 480;
    int winWidth = 640;

    std::vector<std::vector<int>> worldMap;
    std::vector<std::vector<int>> floorMap;
    std::vector<std::vector<int>> ceilingMap;
    uint8_t textures[7][32 * 32 * 3];

    float posX = 21, posY = 2;                  // x and y start position
    float dirX = -1, dirY = 0, oldDirX;         // initial direction vector
    float planeX = 0, planeY = 0.66, oldPlaneX; // the 2d raycaster version of camera plane

    float moveSpeed = .1;
    float rotSpeed = .04;

    int texWidth = 32, texHeight = 32;

    bool w_pressed = false, s_pressed = false, a_pressed = false, d_pressed = false, e_pressed = false;

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
    bool running() { return isRunning; }
    // getter and setter for window height and width
    int getWinHeight() { return winHeight; }
    int getWinWidth() { return winWidth; }
    void setWinHeight(int h) { winHeight = h; }
    void setWinWidth(int w) { winWidth = w; }
    void renderWorld();
    void movement();
};

#endif