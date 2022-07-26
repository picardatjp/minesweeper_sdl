#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "Game.hpp"
#include "Piece.hpp"
#include <iostream>
#include <random>
#include <ctime>
#include <set>
#include <vector>

const char *tiles_path = "res/tiles.png";
Game *game = nullptr;

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
        // we create the window here
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (!window)
            std::cout << "Failed to create window." << std::endl;

        // create our renderer (not sure what the -1 and 0 are)
        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
    // get our texture for our game pieces
    SDL_Surface *temp_surf = IMG_Load(tiles_path);
    texture = SDL_CreateTextureFromSurface(renderer, temp_surf);
    SDL_FreeSurface(temp_surf);
    // r will be temp rect to get the location of each piece in our texture
    SDL_Rect r;
    // all 64x64px
    r.w = r.h = selected_rect.h = selected_rect.w = 64;
    // we have twelve pieces
    r.y = 0;
    for (int i = 0; i < 12; i++)
    {
        // each is 64x64px all horizontal, so y doesn't change
        r.x = i * 64;
        // add it to our tile_src array
        tile_src[i] = r;
    }
}

void Screen::handleEvents()
{
    // create an event
    SDL_Event event;
    // poll the event
    SDL_PollEvent(&event);
    // stores keyboard button presses
    const Uint8 *state;
    // x and y for mouse position. overtile_x,y for which tile we are dropping our piece on
    int x, y, over_tile_x, over_tile_y;

    // find what events happened
    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running = false;
        break;
    case SDL_KEYDOWN:
        // a button was pressed so we want which ones
        break;
    // we pressed down the mouse button
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT && !mouse_down)
        {
            // get mouse position stored in x and y
            SDL_GetMouseState(&x, &y);
            // set mouse down flag
            mouse_down = true;
            // store the initial click position
            initMX = x;
            initMY = y;
            // get the offset from our initial click to the upper left corner of the tile we clicked on
            initMX_offset = x - (int(x / TILE_WIDTH)) * TILE_WIDTH;
            initMY_offset = y - (int(y / TILE_HEIGHT)) * TILE_HEIGHT;
            // get the tile we clicked on
            selected_tile = (initMY / TILE_HEIGHT) * 8 + initMX / TILE_WIDTH;
        }
        break;
    // we released the mouse button
    case SDL_MOUSEBUTTONUP:
        // if (event.button.button == SDL_BUTTON_LEFT)
        // {
        //     // no longer holding down the button
        //     mouse_down = false;
        //     if (game->getPiece(selected_tile) != empty)
        //     {
        //         // get mouse position stored in x and y
        //         SDL_GetMouseState(&x, &y);
        //         // the tile that we dropped the middle point of our selected piece on
        //         over_tile_x = int((x - initMX_offset + (TILE_WIDTH / 2)) / TILE_WIDTH);
        //         over_tile_y = int((y - initMY_offset + (TILE_HEIGHT / 2)) / TILE_HEIGHT);
        //     }
        //     // set to something not 0:63 so we draw all pieces on the board again
        //     selected_tile = -1;
        //     // clear the possible moves arrays
        //     possible_moves_vec.clear();
        //     memset(possible_moves_arr, 0, sizeof(possible_moves_arr));
        // }
        break;
    default:
        break;
    }
}

void Screen::update()
{
    // if we have our mouse held down we want to move a tile
    // if (mouse_down)
    // {
    //     moveTile();
    // }
}

void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // render board
    renderBoard();

    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderBoard()
{
    SDL_Rect r;
    r.x = 29;
    r.y = 600;
    r.w = 48;
    r.h = 64;
    for (int i = 0; i < 10; i++)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &r);
        r.x += r.w + 29;
    }
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}