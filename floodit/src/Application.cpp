#include "Application.hpp"
#include "SDL.h"
#include <iostream>

SDL_Renderer *Application::renderer = nullptr;

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
        renderer = SDL_CreateRenderer(window_, -1, 0);
        if (!renderer)
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

// update game objects here eg. sprite locations adn whatnot
void Application::update()
{
    // update positions and states here
}

// our render function which draws to the screen
void Application::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // render stuff here
    SDL_Rect rect = {100, 100, 100, 100};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}