#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

const char *picture_path = "res/picture.png";

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
    SDL_Surface *temp_surf = IMG_Load(picture_path);
    texture = SDL_CreateTextureFromSurface(renderer, temp_surf);
    SDL_FreeSurface(temp_surf);
    SDL_Rect r;
    for (int i = 0; i < 16; i++)
    {
        r.x = (i % 4) * 160;
        r.y = int(i / 4) * 160;
        r.w = r.h = 160;
        tiles[i].src = r;
        tiles[i].dest = r;
        // tileset[(i + 2) % 16] = r;
        // board[i] = r;
    }
    // board = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
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
    // find what events happened

    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running = false;
        break;
    case SDL_KEYDOWN:
        // button was pressed so we want which ones
        state = SDL_GetKeyboardState(NULL);
        // if "B" was pressed
        if (state[SDL_SCANCODE_B])
        {
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            SDL_GetMouseState(&x, &y);
            mouseDown = true;
            initMX = x;
            initMY = y;
            initMX_offset = x - (int(x / 160)) * 160;
            initMY_offset = y - (int(y / 160)) * 160;
            // initTX = board[(initMY / 160) * 4 + (initMX / 160)].x;
            // initTY = board[(initMY / 160) * 4 + (initMX / 160)].y;
            // std::cout << board[(initMY / 160) * 4 + (initMX / 160)].x << " " << initMY;
            // std::cout << "pressed ";
            // std::cout << int(x / 160) << ", " << int(y / 160) << std::endl;
        }
        break;
    // no event happened
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            // std::cout << "released ";
            mouseDown = false;
            snapTile();
        }
        break;
    default:
        break;
    }
}

void Screen::update()
{
    if (mouseDown)
    {
        // std::cout << "here";
        moveTile();
    }
}

void Screen::render()
{
    // clears the screen
    SDL_RenderClear(renderer);
    // render top part of screen
    // renderRibbon();
    // render bar graph
    renderBoard();
    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::moveTile()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    // std::cout << initMX - x << ", " << initMY - y << std::endl;
    int tx = initMX / 160;
    int ty = initMY / 160;
    // board[ty * 4 + tx].x = initTX + (x - initMX);
    // board[ty * 4 + tx].y = initTY + (y - initMY);
    tiles[ty * 4 + tx].dest.x = x - initMX_offset;
    tiles[ty * 4 + tx].dest.y = y - initMY_offset;
}

void Screen::snapTile()
{
    int tx = initMX / 160;
    int ty = initMY / 160;
    // swapTile(board[ty * 4 + tx], board[(int((board[ty * 4 + tx].y + 80) / 160)) * 4 + (int((board[ty * 4 + tx].x + 80) / 160))]);
    // board[ty * 4 + tx].x = (int((board[ty * 4 + tx].x + 80) / 160)) * 160;
    // board[ty * 4 + tx].y = (int((board[ty * 4 + tx].y + 80) / 160)) * 160;
    // board[(int((board[ty * 4 + tx].y + 80) / 160)) * 4 + (int((board[ty * 4 + tx].x + 80) / 160))].x = (int(initMX / 160)) * 160;
    // board[(int((board[ty * 4 + tx].y + 80) / 160)) * 4 + (int((board[ty * 4 + tx].x + 80) / 160))].y = (int(initMY / 160)) * 160;
    // (int((board[ty * 4 + tx].y + 80) / 160)) * 4 + (int((board[ty * 4 + tx].x + 80) / 160))

    int snapToX = (int((tiles[ty * 4 + tx].dest.x + 80) / 160));
    int snapToY = (int((tiles[ty * 4 + tx].dest.y + 80) / 160));
    tiles[ty * 4 + tx].dest.x = (int(initMX / 160)) * 160;
    tiles[ty * 4 + tx].dest.y = (int(initMY / 160)) * 160;
    Tile t = tiles[ty * 4 + tx];
    tiles[ty * 4 + tx] = tiles[snapToY * 4 + snapToX];
    tiles[snapToY * 4 + snapToX] = t;
    SDL_Rect r;
    r = tiles[ty * 4 + tx].dest;
    tiles[ty * 4 + tx].dest = tiles[snapToY * 4 + snapToX].dest;
    tiles[snapToY * 4 + snapToX].dest = r;

    // std::cout << board[1].x << ", " << board[1].y << " : " << board[5].x << ", " << board[5].y << " ";
}

void Screen::swapTile(SDL_Rect &a, SDL_Rect &b)
{
    SDL_Rect temp;
    temp.x = a.x;
    temp.y = a.y;
    a.x = b.x;
    a.y = b.y;
    b.y = temp.y;
    b.x = temp.x;
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

void Screen::renderBoard()
{
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r;

    // go through all values
    for (int i = 0; i < 16; i++)
    {
        // draw a line for each pixel of our bar, this could be replaced with SDL_RenderFillRect() now that i think about it. -2 for padding
        // for (int j = 0; j < BOARD_WIDTH; j++)
        // {
        //     // draw line starting at bottom of screen up to the percent valueof the screen minus the ribbon_offset at the top
        //     // SDLRenderDrawLine(renderer,x1,y1,x2,y2)
        //     r.x = j * getWinWidth() / BOARD_WIDTH;
        //     r.y = i * getWinHeight() / BOARD_HEIGHT;
        //     r.w = getWinWidth() / BOARD_WIDTH;
        //     r.h = getWinHeight() / BOARD_HEIGHT;

        // SDL_RenderFillRect(renderer, i * (getWinWidth() / board.size()) + j + 1, getWinHeight(), i * (getWinWidth() / values.size()) + j + 1, int(getWinHeight() - ((getWinHeight() - ribbon_offset) * percent)));
        // SDL_RenderFillRect(renderer, &tileset[i]);
        SDL_RenderCopy(renderer, texture, &tiles[i].src, &tiles[i].dest);
        // }
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