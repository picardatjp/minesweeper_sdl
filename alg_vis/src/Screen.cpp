#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
// our static renderer
// SDL_Renderer *Screen::renderer = nullptr;

SDL_Texture *buttons_texture;
const char *buttons_texture_path = "res/buttons.png";

Screen::Screen()
{
}

Screen::~Screen()
{
}

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
        is_running = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        is_running = false;
    }
    SDL_Surface *temp_surf = IMG_Load(buttons_texture_path);
    buttons_texture = SDL_CreateTextureFromSurface(renderer, temp_surf);
    SDL_FreeSurface(temp_surf);

    values_size = 32;
    randomizeValues();
    change_time_start = change_time_end = SDL_GetTicks();
    sorted = false;
    sort_recorded = false;
    swaps_index = 0;
    compares_index = 0;
}

void Screen::handleEvents()
{
    // create an event
    SDL_Event event;
    // poll the event
    SDL_PollEvent(&event);
    // stores keyboard button presses
    const Uint8 *state;
    // find what events happened
    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running = false;
        break;
    case SDL_KEYDOWN:
        state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_R])
        {
            sorted = sort_recorded = false;
            swaps.clear();
            compares.clear();
            compares_index = 0;
            swaps_index = 0;
            randomizeValues();
        }
        break;
    // no event happened
    default:
        break;
    }
}

void Screen::update()
{
    if (!sorted)
        updateGraph();
}

void Screen::updateGraph()
{
    if (!sort_recorded)
    {
        bubbleSort(values);
        sort_recorded = true;
    }
    change_time_end = SDL_GetTicks();
    if (change_time_end - change_time_start > 1 * (1000))
    {
        change_time_end = change_time_start = SDL_GetTicks();
        // std::cout << moves.size() << " " << moveIndex << " ";
        if (!(swaps_index == swaps.size()))
        {
            swap(values[swaps[swaps_index].first], values[swaps[swaps_index].second]);
            swaps_index++;
        }
        else
        {
            sorted = true;
            std::cout << "sorted!" << std::endl;
        }
    }
}

void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderRibbon();
    renderGraph();
    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderRibbon()
{
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    SDL_Rect r;
    r.x = r.y = 0;
    r.w = getWinWidth();
    r.h = ribbon_offset;
    SDL_RenderFillRect(renderer, &r);
}

void Screen::renderButtions()
{
}

void Screen::renderGraph()
{
    float percent;
    // std::cout << int(getWinHeight() * percent) << " ";
    for (int i = 0; i < values.size(); i++)
    {
        percent = (float)values[i] / (float)values.size();
        if (swaps[swaps_index].first == i || swaps[swaps_index].second == i)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int j = 0; j < (getWinWidth() / values.size() - 2); j++)
        {
            SDL_RenderDrawLine(renderer, i * (getWinWidth() / values.size()) + j + 1, getWinHeight(), i * (getWinWidth() / values.size()) + j + 1, int(getWinHeight() - ((getWinHeight() - ribbon_offset) * percent)));
        }
    }
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_DestroyTexture(buttons_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

// my array sizes aren't that big so it is okay to copy it
// this way we don't change the actual array, just record the changes it makes
void Screen::bubbleSort(std::vector<int> v)
{
    int c = 0;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.size() - i - 1; j++)
        {
            compares.push_back(std::pair<int, int>(j, j + 1));
            if (v[j] > v[j + 1])
            {
                swaps.push_back(std::pair<int, int>(j, j + 1));
                swap(v[j], v[j + 1]);
                c++;
            }
        }
    }
    // std::cout << " " << c << " moves ";
}

void Screen::swap(int &a, int &b)
{
    int t = a;
    a = b;
    b = t;
}

void Screen::randomizeValues()
{
    unsigned int s = time(0);
    srand(s);
    int x;
    if (values_size > values.size())
        values.resize(values_size, 0);
    for (int i = 0; i < values.size(); i++)
    {
        x = 0;
        while (x < 1)
        {
            x = rand() % values.size();
        }
        values[i] = x;
    }
}