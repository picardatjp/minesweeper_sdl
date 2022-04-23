#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "Textbox.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "SDL_ttf.h"
#include <iostream>
#include <memory>

// event function declarations
void textChanged();
void sayHello();

// SDL_Texture *buttons_texture;
// const char *buttons_texture_path = "res/buttons.png";
std::unique_ptr<Textbox> textbox = std::make_unique<Textbox>();
std::unique_ptr<Button> button = std::make_unique<Button>();
std::unique_ptr<Label> label = std::make_unique<Label>();

Screen::Screen()
{
}

Screen::~Screen()
{
}
//////////////////////////////////////////////////

// instatiate sdl_image and sdl_ttf

//////////////////////////////////////////////////
void Screen::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
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
        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            std::cout << "Failed to initialize SDL_Image" << std::endl;
        if (TTF_Init() == -1)
            std::cout << "Failed to initialize SDL_TTF" << std::endl;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        is_running = false;
    }
    SDL_Color c;
    c.r = c.b = c.g = 0;
    c.a = 255;

    SDL_Rect r;
    r.x = r.y = 100;
    r.w = r.h = 400;
    // textbox->init(renderer, r, textChanged);
    textbox->init(renderer, r, sayHello);
    r.x = r.y = 25;
    r.w = 50;
    r.h = 35;
    button->init(r, r, sayHello, renderer, "garbage");
}

void Screen::handleEvents()
{
    // create an event
    SDL_Event event;
    // poll the event
    SDL_PollEvent(&event);
    // stores keyboard button presses
    const Uint8 *state;
    int x, y;
    SDL_Rect r1, r2;
    r1.w = r1.h = 1;
    // find what events happened
    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running = false;
        break;
    case SDL_KEYDOWN:
        // key was pressed so we want which ones
        state = SDL_GetKeyboardState(NULL);
        // if "B" was pressed
        if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
        {
            for (int i = 4; i < 30; i++)
            {
                if (state[i])
                {
                    textbox->update(renderer, char(i + 61));
                }
            }
            if (state[SDL_SCANCODE_1])
            {
                textbox->update(renderer, char(33));
            }
            if (state[SDL_SCANCODE_2])
            {
                textbox->update(renderer, char(64));
            }
            if (state[SDL_SCANCODE_SLASH])
            {
                textbox->update(renderer, char(63));
            }
        }
        else
        {
            for (int i = 4; i < 30; i++)
            {
                if (state[i])
                {
                    textbox->update(renderer, char(i + 93));
                }
            }
            for (int i = 30; i < 39; i++)
            {
                if (state[i])
                {
                    textbox->update(renderer, char(i + 19));
                }
            }
            if (state[SDL_SCANCODE_0])
            {
                textbox->update(renderer, char(48));
            }
            if (state[SDL_SCANCODE_PERIOD])
            {
                textbox->update(renderer, char(46));
            }
            if (state[SDL_SCANCODE_COMMA])
            {
                textbox->update(renderer, char(44));
            }
        }
        if (state[SDL_SCANCODE_SPACE])
            textbox->update(renderer, char(32));
        if (state[SDL_SCANCODE_BACKSPACE])
            textbox->update(renderer, char(8));
        // if "Q" was pressed
        // else if (state[SDL_SCANCODE_Q])
        // {
        //     // textbox->onPressed();
        //     label->onPressed();
        // }
        // // if "H" was pressed
        // else if (state[SDL_SCANCODE_H])
        // {
        // }
        break;
    case SDL_MOUSEBUTTONUP:
        SDL_GetMouseState(&x, &y);
        r1.x = x;
        r1.y = y;
        r2 = button->getRect();
        if (SDL_HasIntersection(&r1, &r2) == SDL_TRUE)
            button->setState(1);
    // no event happened
    default:
        break;
    }
}

void Screen::update()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    button->update(x, y);
}

void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    // render top part of screen
    // renderRibbon();
    textbox->render(renderer);
    button->render(renderer);
    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderRibbon()
{
    // set draw color to grey
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    // make a rect from top left to far right and down 64px, or ribbon_offset
    SDL_Rect r;
    r.x = r.y = 0;
    r.w = getWinWidth();
    r.h = ribbon_offset;
    // draw a filled in rectangle using our rect r
    SDL_RenderFillRect(renderer, &r);
}

void Screen::renderButtions()
{
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    // SDL_DestroyTexture(buttons_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

void textChanged()
{
    std::cout << "label event" << std::endl;
}

void sayHello()
{
    std::cout << "button event" << std::endl;
}