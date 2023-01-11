#include "Application.hpp"
#include "SDL.h"
#include <iostream>
#include <vector>
#include "Gate.hpp"
#include <unordered_map>

void drawCircle(SDL_Renderer *r, int cx, int cy, int radius);

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
    Gate g1, g2;
    g1.setType(G_AND);
    g1.setRect({100, 100, 100, 100});
    gates_.push_back(g1);
    g2.setType(G_NOR);
    g2.setRect({300, 300, 100, 100});
    g2.setDir(G_NORTH);
    g2.connectInputA(0);
    gates_.push_back(g2);
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
    drawWires();
    drawGates();

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

void Application::drawGates()
{
    for (int i = 0; i < gates_.size(); i++)
    {
        SDL_Color c = gates_[i].getColor();
        SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, c.a);
        SDL_RenderDrawRect(renderer_, gates_[i].getRect());
    }
}

void Application::drawWires()
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
    for (int i = 0; i < gates_.size(); i++)
    {
        if (gates_[i].getDir() == G_NORTH)
        {
            int x1, x2, y1, y2 = 0;
            int iai = gates_[i].getiai();
            int ibi = gates_[i].getibi();
            if (iai != -1)
            {
                SDL_Rect *r1 = gates_[i].getRect();
                SDL_Rect *r2 = gates_[iai].getRect();
                x1 = (int)(r1->x + (r1->w >> 2));
                y1 = (int)(r1->y + r1->h);
                x2 = (int)(r2->x + (r2->w >> 1));
                y2 = (int)(r2->y);
                SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
            }
            if (ibi != -1)
            {
                SDL_Rect *r1 = gates_[i].getRect();
                SDL_Rect *r2 = gates_[ibi].getRect();
                x1 = (int)(r1->x - (r1->w >> 2) + r1->w);
                y1 = (int)(r1->y + r1->h);
                x2 = (int)(r2->x + (r2->w >> 1));
                y2 = (int)(r2->y);
                SDL_RenderDrawLine(renderer_, x1, r1->y, r2->x, r2->y);
            }
        }
        else if (gates_[i].getDir() == G_SOUTH)
        {
            int x1, x2, y1, y2 = 0;
            int iai = gates_[i].getiai();
            int ibi = gates_[i].getibi();
            if (iai != -1)
            {
                SDL_Rect *r1 = gates_[i].getRect();
                SDL_Rect *r2 = gates_[iai].getRect();
                x1 = (int)(r1->x + (r1->w >> 2));
                y1 = (int)(r1->y + r1->h);
                x2 = (int)(r2->x + (r2->w >> 1));
                y2 = (int)(r2->y);
                SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
            }
            if (ibi != -1)
            {
                SDL_Rect *r1 = gates_[i].getRect();
                SDL_Rect *r2 = gates_[ibi].getRect();
                x1 = (int)(r1->x - (r1->w >> 2) + r1->w);
                y1 = (int)(r1->y + r1->h);
                x2 = (int)(r2->x + (r2->w >> 1));
                y2 = (int)(r2->y);
                SDL_RenderDrawLine(renderer_, x1, r1->y, r2->x, r2->y);
            }
        }
        else if (gates_[i].getDir() == G_EAST)
        {
        }
        else if (gates_[i].getDir() == G_WEST)
        {
        }
    }
}