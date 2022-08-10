#include "Application.hpp"
#include "SDL.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <random>

std::random_device rd;

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
    bird_rect_ = {100, 100, 40, 40};
    int hello = 200;
    for (int i = 0; i < 3; i++)
    {
        // rect = { x y w h }
        pillar_rects_.push_back({hello, 0, pillar_width_, 100});
        pillar_rects_.push_back({hello, 200, pillar_width_, 100});
        hello += 150;
    }
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> randomize(0, WINDOW_HEIGHT_ - pillar_gap_);
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
    case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !space_down_)
        {
            space_down_ = true;
        }
        break;

    case SDL_KEYUP:
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            space_down_ = false;
            jump_reset_ = true;
        }
        break;
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

    if (space_down_ && jump_reset_)
    {
        y_v_ = -10.0f;
        jump_reset_ = false;
    }
    y_v_ += .6f;
    bird_rect_.y += (int)y_v_;
    if (bird_rect_.y >= WINDOW_HEIGHT_ - bird_rect_.h)
    {
        bird_rect_.y = WINDOW_HEIGHT_ - bird_rect_.h;
    }
    if (bird_rect_.y <= 0)
    {
        bird_rect_.y = 1;
        y_v_ = 0.0f;
    }
    checkPillarBounds();
    for (int i = 0; i < pillar_rects_.size(); i++)
    {
        pillar_rects_[i].x -= 3;
    }
}

// our render function which draws to the screen
void Application::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);

    // render stuff here

    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 0);
    SDL_RenderFillRect(renderer_, &bird_rect_);
    SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 0);
    for (SDL_Rect p : pillar_rects_)
    {
        SDL_RenderFillRect(renderer_, &p);
    }

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

void Application::checkPillarBounds()
{
    for (int i = 0; i < pillar_rects_.size(); i += 2)
    {
        if (pillar_rects_[i].x + pillar_width_ <= 0)
        {
            randomizePillarY(i);
            pillar_rects_[i].x = WINDOW_WIDTH_;
            pillar_rects_[i + 1].x = WINDOW_WIDTH_;
        }
    }
}

void Application::randomizePillarY(int p)
{
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> randomize(0, WINDOW_HEIGHT_ - pillar_gap_ - 100);

    int new_height = randomize(rd);
    pillar_rects_[p].h = new_height + 50;
    pillar_rects_[p + 1].y = pillar_rects_[p].h + pillar_gap_;
    pillar_rects_[p + 1].h = WINDOW_HEIGHT_ - pillar_rects_[p + 1].y;
}