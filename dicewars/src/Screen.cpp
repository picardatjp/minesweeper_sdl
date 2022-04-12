#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "Game.hpp"
#include <iostream>

// global renderer pointer
SDL_Renderer *Screen::renderer = nullptr;

const char *texture_path = "res/tiles.png";
SDL_Texture *texture;

Game *game = nullptr;

// Game Constructor
Screen::Screen()
{
}

// Game Destructor
Screen::~Screen()
{
}

// Game initializer function
void Screen::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    // flags
    int flags = 0;
    if (fullscreen)
    {
        // add fullscreen to our flags
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // SDL_Init returns 0 if everything went well
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        // std::cout << "SDL initialized successfully." << std::endl;

        // we create the window here
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (!window)
            std::cout << "Failed to create window." << std::endl;

        // create our renderer (not sure what the -1 and 0 are)
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            // set the screen to black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            // std::cout << "Renderer created successfully." << std::endl;
        }
        else
            std::cout << "Failed to create renderer." << std::endl;

        // we made it here so the game is now running
        isRunning = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        isRunning = false;
    }

    // start game here
    game = new Game();
    SDL_Surface *ts = IMG_Load(texture_path);
    texture = SDL_CreateTextureFromSurface(renderer, ts);
    SDL_FreeSurface(ts);
}

void Screen::handleEvents()
{
    // create an event
    SDL_Event event;
    // poll the event
    SDL_PollEvent(&event);
    // find what events happened
    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        isRunning = false;
        break;
    // no event happened
    default:
        break;
    }
}

void Screen::update()
{
}

void Screen::render()
{
    // clears the screen
    SDL_RenderClear(renderer);
    SDL_Rect r;
    r.x = r.y = 0;
    r.w = 36;
    r.h = 32;
    // SDL_RenderCopy(renderer, texture, &r, &r);
    renderScreen();
    //  actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderScreen()
{
    SDL_Rect src, dest;
    src.w = 36;
    src.h = 32;
    src.x = 36;
    src.y = 0;
    dest.x = dest.y = 0;
    dest.w = 36;
    dest.h = 32;
    for (int i = 0; i < 5; i++)
    {
        dest.y = dest.y + 16;
        dest.x = 0;
        for (int j = 0; j < 10; j++)
        {
            dest.x = dest.x + 56;
            SDL_RenderCopy(renderer, texture, &src, &dest);
        }
        dest.y = dest.y + 16;
        dest.x = 28;
        for (int j = 0; j < 10; j++)
        {
            dest.x = dest.x + 56;
            SDL_RenderCopy(renderer, texture, &src, &dest);
        }
    }
    // std::cout << "hello";
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}