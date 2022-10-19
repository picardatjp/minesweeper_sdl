#include "Application.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

const char *blue_path = "../res/blue_b.png";
const char *bg_path = "../res/map1.png";

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
        if (!IMG_Init(IMG_INIT_PNG))
            std::cout << "problem"
                      << "\n";
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

    // the path for map1
    s_.points.push_back({-50, 80});
    s_.points.push_back({-49, 80});
    s_.points.push_back({730, 85});
    s_.points.push_back({750, 290});
    s_.points.push_back({160, 280});
    s_.points.push_back({165, 490});
    s_.points.push_back({470, 510});
    s_.points.push_back({540, 425});
    s_.points.push_back({940, 435});
    s_.points.push_back({941, 435});

    createSpline(line);

    Bloon b;
    SDL_Surface *temp_surf = IMG_Load(blue_path);
    b.setTexture(SDL_CreateTextureFromSurface(renderer_, temp_surf));
    temp_surf = IMG_Load(bg_path);
    bg = SDL_CreateTextureFromSurface(renderer_, temp_surf);
    SDL_FreeSurface(temp_surf);
    b.setSrc({0, 0, 60, 80});
    b.setDest({0, 0, 60, 80});
    bloons.push_back(b);
    bloons[0].setPI(4);
    // for (int i = 0; i < line.size(); i++)
    // {
    //     // std::cout << line[i].x << " | " << line[i].y << "\n";
    // }
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
    SDL_Rect r = {0, 0, 900, 600};
    SDL_RenderCopy(renderer_, bg, &r, NULL);
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 0);
    for (int i = 0; i < line.size(); i++)
    {
        SDL_RenderDrawPoint(renderer_, line[i].x, line[i].y);
    }

    // SDL_Rect r = {0, 0, 60, 80};
    for (int i = 0; i < bloons.size(); i++)
    {
        bloons[i].setPI(bloons[i].getPI() + 1);
        bloons[i].dest.x = (int)(line[bloons[i].getPI()].x) - (bloons[i].src.w >> 1);
        bloons[i].dest.y = (int)(line[bloons[i].getPI()].y) - (bloons[i].src.h >> 1);
    }
    for (int i = 0; i < bloons.size(); i++)
    {
        SDL_RenderCopy(renderer_, bloons[i].getTex(), &(bloons[i].src), &(bloons[i].dest));
        if (bloons[i].getPI() > line.size())
            bloons[i].setPI(0);
    }
    // std::cout << bloons[0].getPI() << "\n";
    // if (count >= line.size())
    // {
    //     count = 0;
    // }
    SDL_RenderPresent(renderer_);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    for (int i = 0; i < bloons.size(); i++)
    {
        SDL_DestroyTexture(bloons[i].getTex());
    }
    SDL_Quit();
}

float Application::lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

void Application::createSpline(std::vector<point> &line)
{
    for (float t = 0.0f; t < (float)s_.points.size() - 3.0f; t += 0.001f)
    {
        point pos = s_.getSplinePoint(t);
        if (line.empty())
        {
            line.push_back(pos);
        }
        if ((int)(line[line.size() - 1].x) != (int)(pos.x) || (int)(line[line.size() - 1].y) != (int)(pos.y))
        {
            line.push_back({pos.x, pos.y});
        }
    }
}
