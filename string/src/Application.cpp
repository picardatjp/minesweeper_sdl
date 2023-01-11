#include "Application.hpp"
#include "SDL.h"
#include <iostream>

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
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);

    // render stuff here
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 0);
    drawString();

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

// SDL_Point Application::stringSineY(float y, float a)
// {
//     int x = int(a * sin(int(y) * 3.1416f / 180) + (WINDOW_WIDTH_ >> 1));
//     return {x, int(y)};
// }

void Application::drawString()
{
    float pos_y = sx_;
    float pos_x = 0.0f;
    float pos_y_p = 0.0f;
    float pos_x_p = 0.0f;
    while (int(pos_y) < WINDOW_HEIGHT_)
    {
        if ((int(pos_y) != int(pos_y_p)) || (int(pos_x) != int(pos_x_p)))
        {
            SDL_RenderDrawPoint(renderer_, int(pos_x), int(pos_y));
            pos_x_p = pos_x;
            pos_y_p = pos_y;
        }
        pos_y += 0.005f;
        pos_x = amp_ * sin(freq2_ * (pos_y)*3.1416f / 180) + (WINDOW_WIDTH_ >> 1) + amp2_ * sin(freq1_ * (pos_y)*3.1416f / 180);
    }
    sx_ = sx_ + 1;
    if (freq1_ > 11.0f || freq1_ < 1.0f)
    {
        freq1i_ *= -1;
    }
    if (amp_ > 100.0f || amp_ < 90.0f)
    {
        amp1i_ *= -1;
    }
    amp_ += amp1i_;
    freq1_ += freq1i_;
    std::cout << freq1_ << " " << freq1i_ << "\n";
}