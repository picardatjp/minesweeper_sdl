#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

const char *car_path = "res/car.png";
SDL_Texture *car_texture = nullptr;

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
        is_running_ = true;
        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
            std::cout << "Failed to initialize SDL_Image" << std::endl;
        // if (TTF_Init() == -1)
        //     std::cout << "Failed to initialize SDL_TTF" << std::endl;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        is_running_ = false;
    }
    SDL_Surface *temp = IMG_Load(car_path);
    car_texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    car_dest_.x = car_dest_.y = 0;
    car_dest_.w = car_dest_.h = 128;
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
        is_running_ = false;
        break;
    case SDL_KEYDOWN:
        // key was pressed so we want which ones
        state = SDL_GetKeyboardState(NULL);
        // if "B" was pressed
        if (state[SDL_SCANCODE_LEFT])
        {
            left_down_ = true;
        }
        if (state[SDL_SCANCODE_RIGHT])
        {
            right_down_ = true;
        }
        if (state[SDL_SCANCODE_UP])
        {
            up_down_ = true;
        }
        if (state[SDL_SCANCODE_DOWN])
        {
            down_down_ = true;
        }
        break;
    case SDL_KEYUP:
        state = SDL_GetKeyboardState(NULL);
        // if "B" was pressed
        if (!state[SDL_SCANCODE_LEFT])
        {
            left_down_ = false;
        }
        if (!state[SDL_SCANCODE_RIGHT])
        {
            right_down_ = false;
        }
        if (!state[SDL_SCANCODE_UP])
        {
            up_down_ = false;
        }
        if (!state[SDL_SCANCODE_DOWN])
        {
            down_down_ = false;
        }
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

    // if (mouse_release_)
    // {
    //     int x, y;
    //     SDL_GetMouseState(&x, &y);
    //     button->update(x, y);
    //     menu->update(x, y);
    //     mouse_release_ = false;
    // }
    moveCar();
}

void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    renderCar();
    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderCar()
{
    SDL_RenderCopy(renderer, car_texture, NULL, &car_dest_);
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    // SDL_DestroyTexture(buttons_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    // TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

// clunky and goes past max and min
void Screen::moveCar()
{
    if (left_down_)
    {
        // move to the left
        car_x_speed_ -= float(SPEED_ / 60.0f);
    }
    if (right_down_)
    {
        // move to the right
        car_x_speed_ += float(SPEED_ / 60.0f);
    }
    if (up_down_)
    {
        // move up
        car_y_speed_ -= float(SPEED_ / 60.0f);
    }
    if (down_down_)
    {
        // move down
        car_y_speed_ += float(SPEED_ / 60.0f);
    }
    if (car_x_speed_ > max_speed_)
        car_x_speed_ = max_speed_;
    else if (car_y_speed_ < min_speed_)
        car_x_speed_ = min_speed_;
    if (car_y_speed_ > max_speed_)
        car_y_speed_ = max_speed_;
    else if (car_y_speed_ < min_speed_)
        car_y_speed_ = min_speed_;
    car_x_ += car_x_speed_;
    car_y_ += car_y_speed_;
    car_dest_.x = int(car_x_);
    car_dest_.y = int(car_y_);

    std::cout << "x: " << car_x_speed_ << " y: " << car_y_speed_ << "\n";
}