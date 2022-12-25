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
    int x, y;
    // poll the event
    SDL_PollEvent(&event);
    // find what events happened
    switch (event.type)
    {
    case SDL_MOUSEBUTTONUP:
        SDL_GetMouseState(&x, &y);
        // i was here when i stopped
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
    drawGrid();
    drawPieces();

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

void Application::drawGrid()
{
    SDL_SetRenderDrawColor(renderer_, 127, 255, 255, 255);
    SDL_Rect row = {border_buffer_, border_buffer_, WINDOW_WIDTH_ - (border_buffer_ * 2), 5};
    SDL_Rect column = {border_buffer_, border_buffer_, 5, WINDOW_HEIGHT_ - (border_buffer_ * 2)};
    int cell_width = (WINDOW_WIDTH_ - (border_buffer_ * 2)) / board_width_;
    int cell_height = (WINDOW_HEIGHT_ - (border_buffer_ * 2)) / board_height_;
    for (int i = 0; i < board_height_ + 1; i++)
    {
        SDL_RenderFillRect(renderer_, &row);
        row.y += cell_height;
    }
    for (int i = 0; i < board_width_ + 1; i++)
    {
        SDL_RenderFillRect(renderer_, &column);
        column.x += cell_width;
    }
}
void Application::drawPieces()
{
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);

    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
}