#include "Application.hpp"
#include "Button.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

const char *btn_tex = "../res/btn_bg.png";

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
        IMG_Init(IMG_INIT_PNG);
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
    btn_.setOnClick([]()
                    { std::cout << "hello!!!!!" << std::endl; });

    // instantiate stuff here
    SDL_Surface *ts = IMG_Load(btn_tex);
    btn_.setTexture(SDL_CreateTextureFromSurface(renderer_, ts));
    SDL_FreeSurface(ts);
    btn_.src = {0, 0, 100, 100};
    btn_.dest = {300, 300, 100, 100};
}

// this is where events are handled
void Application::handleEvents()
{
    // create an event
    SDL_Event event;
    const Uint8 *state = NULL;
    int x, y;
    SDL_Rect r = {0, 0, 1, 1};
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
    case SDL_MOUSEBUTTONUP:
        SDL_GetMouseState(&x, &y);
        r.x = x;
        r.y = y;
        if (SDL_HasIntersection(&btn_.dest, &r) == SDL_TRUE)
        {
            btn_.onClick();
        }
        break;
    default:
        break;
    }
}

// update game objects here
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
    SDL_Rect rect = {100, 100, 100, 100};
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 0);
    SDL_RenderFillRect(renderer_, &rect);
    btn_.render(renderer_);

    SDL_RenderPresent(renderer_);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyTexture(btn_.getTexture());
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    IMG_Quit();
    SDL_Quit();
}