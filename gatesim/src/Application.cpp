#include "Application.hpp"
#include "SDL.h"
#include <iostream>
#include "Gate.hpp"

Gate g1, g2;
void drawCircle(SDL_Renderer *r, int cx, int cy, int radius);

struct mg
{
    Gate g;
};

// Application Constructor
Application::Application()
{
}

// Application Destructor
Application::~Application()
{
}

// Application initializer function
void Application::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
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
        // we create the window here
        window_ = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (!window_)
            std::cout << "Failed to create window." << std::endl;

        // create our renderer (not sure what the -1 and 0 are)
        renderer_ = SDL_CreateRenderer(window_, -1, 0);
        if (!renderer_)
            std::cout << "Failed to create renderer." << std::endl;

        // we made it here so the game is now running
        is_running_ = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        is_running_ = false;
    }

    // instantiate stuff here

    g1.setType(G_AND);
    g2.setType(G_NOR);
    g1.setRect({100, 100, 100, 100});
    g2.setRect({100, 100, 100, 100});
}

// this is where events like input are handled
void Application::handleEvents()
{
    // create an event
    SDL_Event event;
    const Uint8 *state = NULL;
    // poll the event
    SDL_PollEvent(&event);
    // find what events happened
    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running_ = false;
        break;
    default:
        break;
    }
}

// update game objects here eg. sprite locations and whatnot
void Application::update()
{
    // update positions and states here
}

// our render function which draws to the screen
void Application::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);

    // render stuff here
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 0);
    SDL_RenderDrawRect(renderer_, g1.getRect());
    SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 0);

    // drawCircle(renderer_, 300, 300, 25);
    SDL_RenderPresent(renderer_);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}

// middle point from stack overflow
void drawCircle(SDL_Renderer *r, int cx, int cy, int radius)
{
    // SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    int d = radius * 2;
    int x = radius - 1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = tx - d;
    while (x >= y)
    {
        SDL_RenderDrawPoint(r, cx + x, cy - y);
        SDL_RenderDrawPoint(r, cx + x, cy + y);
        SDL_RenderDrawPoint(r, cx - x, cy - y);
        SDL_RenderDrawPoint(r, cx - x, cy + y);
        SDL_RenderDrawPoint(r, cx + y, cy - x);
        SDL_RenderDrawPoint(r, cx + y, cy + x);
        SDL_RenderDrawPoint(r, cx - y, cy - x);
        SDL_RenderDrawPoint(r, cx - y, cy + x);
        if (error <= 0)
        {
            y++;
            error += ty;
            ty += 2;
        }
        if (error > 0)
        {
            x--;
            tx += 2;
            error += tx - d;
        }
    }
}