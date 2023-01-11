#include "Application.hpp"
#include "Button.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <functional>

const char *btn_tex = "../res/btn_bg.png";
const char *fp = "../res/fonts/bahnschrift.ttf";
const char *main_menu_bg_path = "../res/main_menu_bg.png";

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
        TTF_Init();
        if (!renderer_)
            std::cout
                << "Failed to create renderer." << std::endl;

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

    SDL_Surface *ts = IMG_Load(btn_tex);
    play_btn_.init({0, 0, 100, 100}, {300, 300, 100, 100}, SDL_CreateTextureFromSurface(renderer_, ts), "play", renderer_);
    play_btn_.setOnClick([this]
                         { std::cout << "hello!!!!!" << std::endl;
                         main_menu_visible_=false;
                        game_visible_=true; });
    back_to_main_btn_.init({0, 0, 100, 100}, {10, 10, 100, 100}, SDL_CreateTextureFromSurface(renderer_, ts), "main menu", renderer_);
    back_to_main_btn_.setOnClick([this]
                                 {main_menu_visible_=true;game_visible_=false; });
    ts = IMG_Load(main_menu_bg_path);
    main_menu_bg_ = SDL_CreateTextureFromSurface(renderer_, ts);
    SDL_FreeSurface(ts);
    main_menu_label_.init(renderer_, "Main Menu", fp, 72, {0, 0, 0, 255}, {WINDOW_WIDTH_ >> 1, WINDOW_HEIGHT_ >> 2, 0, 0});
    game_label_.init(renderer_, "this is the game screen!", fp, 72, {255, 255, 0, 255}, {WINDOW_WIDTH_ >> 1, WINDOW_HEIGHT_ >> 1, 0, 0});
    c_.init(30, {50, 50}, {255, 255, 0, 255}, {50, 100, 255, 255}, 2, true, true);
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
        if (game_visible_)
        {
            if (SDL_HasIntersection(&back_to_main_btn_.dest, &r) == SDL_TRUE)
            {
                back_to_main_btn_.onClick();
            }
        }
        if (main_menu_visible_)
        {
            if (SDL_HasIntersection(&play_btn_.dest, &r) == SDL_TRUE)
            {
                play_btn_.onClick();
            }
            // if (SDL_HasIntersection(&settings_btn_.dest, &r) == SDL_TRUE)
            // {
            //     settings_btn_.onClick();
            // }
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
    // if screen content modified, render. otherwise dont bother rendering and wasting cpu
    // modified = true;
}

// our render function which draws to the screen
void Application::render()
{
    // if(modified){render everything} else {dont}
    // clears the screen
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);
    if (game_visible_)
    {
        back_to_main_btn_.render(renderer_);
        game_label_.render(renderer_);
    }
    else if (main_menu_visible_)
    {
        // background
        SDL_RenderCopy(renderer_, main_menu_bg_, NULL, NULL);
        play_btn_.render(renderer_);
        // settings_btn_.render(renderer_);
        main_menu_label_.render(renderer_);
        c_.render(renderer_);
    }
    else if (settings_visible_)
    {
    }

    SDL_RenderPresent(renderer_);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    play_btn_.free();
    settings_btn_.free();
    main_menu_label_.free();
    game_label_.free();
    SDL_DestroyTexture(main_menu_bg_);
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    IMG_Quit();
    SDL_Quit();
}