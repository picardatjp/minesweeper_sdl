#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "Textbox.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "SDL_ttf.h"
#include "Menu.hpp"
#include <iostream>
#include <memory>

// event function declarations
void textChanged();
void clearButton();
void btn1pressed();
void btn2pressed();
void btn3pressed();

// SDL_Texture *buttons_texture;
// const char *buttons_texture_path = "res/buttons.png";
std::unique_ptr<Textbox> textbox = std::make_unique<Textbox>();
std::unique_ptr<Button> button = std::make_unique<Button>();
std::unique_ptr<Label> label = std::make_unique<Label>();
std::unique_ptr<Menu> menu = std::make_unique<Menu>();

SDL_Renderer *gross_global_renderer = nullptr;

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
    gross_global_renderer = renderer;
    SDL_Rect r;
    r.x = r.y = 100;
    r.w = r.h = 400;
    // textbox->init(renderer, r, textChanged);
    textbox->init(renderer, r, clearButton);
    r.x = r.y = 40;
    r.w = 60;
    r.h = 40;
    SDL_Color c1, c2;
    c1.r = c1.b = c1.g = c2.b = c2.g = 0;
    c1.a = c2.a = 255;
    c2.r = 255;
    button->init(renderer, r, "clear", c2, c1, clearButton);
    c2.r = 240;
    c2.g = 215;
    c2.b = 180;
    menu->init(getWinWidth(), 30, c2);
    c2.r = 255;
    c2.g = 0;
    c2.b = 0;
    menu->addButton(renderer, 80, "btn1", c2, c1, btn1pressed);
    c2.r = 0;
    c2.g = 255;
    c2.b = 0;
    menu->addButton(renderer, 80, "btn2", c2, c1, btn2pressed);
    c2.r = 255;
    c2.g = 255;
    c2.b = 0;
    menu->addButton(renderer, 80, "btn3", c2, c1, btn3pressed);
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
        //     // textbox->onClicked();
        //     label->onClicked();
        // }
        // // if "H" was pressed
        // else if (state[SDL_SCANCODE_H])
        // {
        // }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (!mouse_down_)
            mouse_down_ = true;
    case SDL_MOUSEBUTTONUP:
        if (mouse_down_)
        {
            mouse_down_ = false;
            mouse_release_ = true;
        }
    // no event happened
    default:
        break;
    }
}

void Screen::update()
{
    // if (mouse_down_)

    if (mouse_release_)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        button->update(x, y);
        menu->update(x, y);
        mouse_release_ = false;
    }
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
    menu->render(renderer);
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
    std::cout << "textbox event" << std::endl;
}

void clearButton()
{
    textbox->setText(gross_global_renderer, "");
    std::cout << "text cleared." << std::endl;
}
void btn1pressed()
{
    std::cout << "btn1" << std::endl;
}
void btn2pressed()
{
    std::cout << "btn2" << std::endl;
}
void btn3pressed()
{
    std::cout << "btn3" << std::endl;
}