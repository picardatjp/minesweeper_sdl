#include "Application.hpp"
#include "SDL.h"
#include <iostream>
#include <vector>

SDL_Surface *screen;
SDL_Surface *screen_save;
spline path;

Application::Application()
{
}

Application::~Application()
{
}
void Application::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    // flags
    int flags = 0;
    flags |= SDL_WINDOW_RESIZABLE;
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
        window_ = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

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
    screen = SDL_GetWindowSurface(window_);
    screen_save = SDL_ConvertSurface(screen, screen->format, 0);
    // if (SDL_BlitSurface(screen, NULL, screen_save, NULL) != 0)
    // {
    //     std::cout << SDL_GetError();
    // }
}

void Application::handleEvents()
{
    int x, y;
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
        is_running_ = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_A)
        {
        }
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            std::cout << "left button clicked\n";
            mouse_down_ = true;
            if (SDL_BlitSurface(screen, NULL, screen_save, NULL))
            {
                std::cout << "didnt worksssss";
            }
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            std::cout << "left button released\n";
            mouse_down_ = false;
            if (path.points.size() >= 4)
            {
                // spline
                for (float t = 0.0f; t < (float)path.points.size() - 3.0f; t += 0.005f)
                {
                    point pos = path.getSplinePoint(t);
                    setPixel((int)pos.x, (int)pos.y, screen_save);
                    SDL_BlitSurface(screen_save, NULL, screen, NULL);
                    SDL_RenderDrawPoint(renderer_, (int)x, (int)y);
                }
            }
            else if (path.points.size() == 3)
            {
                // quadratic bezier
                for (float t = 0.0f; t < 1.0f; t += 0.005f)
                {
                    float x1 = lerp(path.points[0].x, path.points[1].x, t);
                    float x2 = lerp(path.points[1].x, path.points[2].x, t);
                    float y1 = lerp(path.points[0].y, path.points[1].y, t);
                    float y2 = lerp(path.points[1].y, path.points[2].y, t);
                    point pos = {lerp(x1, x2, t), lerp(y1, y2, t)};
                    setPixel((int)pos.x, (int)pos.y, screen_save);
                    SDL_BlitSurface(screen_save, NULL, screen, NULL);
                }
            }
            else if (path.points.size() == 2)
            {
                // linear interpolation
                for (float t = 0.0f; t < 1.0f; t += 0.005f)
                {
                    point pos = {lerp(path.points[0].x, path.points[1].x, t), lerp(path.points[0].y, path.points[1].y, t)};
                    setPixel((int)pos.x, (int)pos.y, screen_save);
                    SDL_BlitSurface(screen_save, NULL, screen, NULL);
                }
            }
            path.points.clear();
        }
        break;
    default:
        break;
    }
}

void Application::update()
{
    if (mouse_down_)
    {
        int x, y;
        Uint32 mouse_state = SDL_GetMouseState(&x, &y);
        std::cout << "(" << x << ", " << y << ")\n";
        setPixel(x, y, screen);
        if (path.points.size() > 0)
        {
            if ((path.points[path.points.size() - 1].x != x) || (path.points[path.points.size() - 1].y != y))
                path.points.push_back({x * 1.0f, y * 1.0f});
        }
        else
            path.points.push_back({x * 1.0f, y * 1.0f});
    }
}

void Application::render()
{
    SDL_UpdateWindowSurface(window_);
}

void Application::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_FreeSurface(screen);
    SDL_FreeSurface(screen_save);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Application::setPixel(int x, int y, SDL_Surface *s)
{
    if (x > 0 && y > 0 && x < 640 && y < 480)
    {
        SDL_LockSurface(s);
        Uint8 *pixel = (Uint8 *)s->pixels + y * s->pitch + x * s->format->BytesPerPixel;
        *pixel = 255;
        pixel++;
        *pixel = 255;
        pixel++;
        *pixel = 0;
        pixel++;
        *pixel = 255;
        SDL_UnlockSurface(s);
    }
}

float Application::lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}