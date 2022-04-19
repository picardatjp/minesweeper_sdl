#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "Game.hpp"
#include "Piece.hpp"
#include <iostream>
#include <random>
#include <ctime>
#include <set>

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

    // create our game object
    game = new Game();
    game->newBoard();
    // get our texture for our game pieces
    SDL_Surface *temp_surf = IMG_Load(tiles_path);
    texture = SDL_CreateTextureFromSurface(renderer, temp_surf);
    SDL_FreeSurface(temp_surf);
    // r will be temp rect to get the location of each piece in our texture
    SDL_Rect r;
    // all 64x64px
    r.w = r.h = selected_rect.h = selected_rect.w = TILE_HEIGHT;
    // we have twelve pieces
    r.y = 0;
    for (int i = 0; i < 12; i++)
    {
        // each is 64x64px all horizontal, so y doesn't change
        r.x = i * TILE_WIDTH;
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
        state = SDL_GetKeyboardState(NULL);
        // if "Q" was pressed
        if (state[SDL_SCANCODE_Q])
        {
            game->newBoard();
        }
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
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            // no longer holding down the button
            mouse_down = false;
            if (game->getPiece(selected_tile) != empty)
            {
                // get mouse position stored in x and y
                SDL_GetMouseState(&x, &y);
                // the tile that we dropped the middle point of our selected piece on
                over_tile_x = int((x - initMX_offset + (TILE_WIDTH / 2)) / TILE_WIDTH);
                over_tile_y = int((y - initMY_offset + (TILE_HEIGHT / 2)) / TILE_HEIGHT);
                // if out of bounds
                if (over_tile_x > 7)
                    over_tile_x = 7;
                if (over_tile_y > 7)
                    over_tile_y = 7;
                // if the move we tried to make is legal
                if (game->isValidMove(selected_tile, over_tile_x + over_tile_y * 8) && selected_tile != (over_tile_x + over_tile_y * 8))
                {
                    // then we actually move it to that square
                    game->movePiece(selected_tile, over_tile_x + over_tile_y * 8);
                }
            }
            // set to something not 0:63 so we draw all pieces on the board again
            selected_tile = -1;
        }
        break;
    default:
        break;
    }
}

void Screen::update()
{
    // if we have our mouse held down we want to move a tile
    if (mouse_down)
    {
        moveTile();
    }
}

void Screen::render()
{
    // clears the screen
    SDL_RenderClear(renderer);

    // render board
    renderBoard();
    // render the piece we have clicked on
    renderSelectedPiece();

    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::moveTile()
{
    // get the x and y position of the mouse
    int x, y;
    SDL_GetMouseState(&x, &y);
    // set the x and y to our mouse - the offset from the top left of the piece we are holding
    selected_rect.x = x - initMX_offset;
    selected_rect.y = y - initMY_offset;
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
    int piece, offset;
    SDL_Rect r;
    // 64x64 squares
    r.w = r.h = TILE_HEIGHT;
    // go through all tiles
    for (int i = 0; i < 64; i++)
    {
        // alternate white and green tiles for board background
        if (i % 2 == 0 && int(i / 8) % 2 == 0 || i % 2 == 1 && int(i / 8) % 2 == 1)
            SDL_SetRenderDrawColor(renderer, 210, 185, 185, 255);
        else
            SDL_SetRenderDrawColor(renderer, 135, 70, 70, 255);
        // set the x and y of our 64x64 rectangle
        r.y = int(i / 8) * TILE_HEIGHT;
        r.x = (i % 8) * TILE_WIDTH;
        // draw the rect to the screen
        SDL_RenderFillRect(renderer, &r);
        // don't draw the piece we are dragging around
        if (i != selected_tile)
        {
            // get the piece type of cell we are looking at
            piece = game->getPiece(i);
            // if it isn't empty
            if (piece > empty)
            {
                // pieces in the texture are same order, black followed by white
                // if white we offset by the number of black pieces, if black we don't offset and start from zero
                if ((piece & white) == white)
                    offset = 6;
                else
                    offset = 0;
                // if piece is king, draw king etc.
                if ((piece & king) == king)
                {
                    SDL_RenderCopy(renderer, texture, &tile_src[5 + offset], &r);
                }
                else if ((piece & queen) == queen)
                {
                    SDL_RenderCopy(renderer, texture, &tile_src[4 + offset], &r);
                }
                else if ((piece & bishop) == bishop)
                {
                    SDL_RenderCopy(renderer, texture, &tile_src[3 + offset], &r);
                }
                else if ((piece & knight) == knight)
                {
                    SDL_RenderCopy(renderer, texture, &tile_src[2 + offset], &r);
                }
                else if ((piece & rook) == rook)
                {
                    SDL_RenderCopy(renderer, texture, &tile_src[1 + offset], &r);
                }
                else if ((piece & pawn) == pawn)
                {
                    SDL_RenderCopy(renderer, texture, &tile_src[0 + offset], &r);
                }
            }
        }
    }
}

void Screen::renderSelectedPiece()
{
    // get the type of piece we are holding
    int piece = game->getPiece(selected_tile);
    int offset;
    // pretty much same code as renderBoard()
    if ((piece & white) == white)
        offset = 6;
    else
        offset = 0;
    // if piece == king, draw king etc, at the selected_rect location
    if ((piece & king) == king)
    {
        SDL_RenderCopy(renderer, texture, &tile_src[5 + offset], &selected_rect);
    }
    else if ((piece & queen) == queen)
    {
        SDL_RenderCopy(renderer, texture, &tile_src[4 + offset], &selected_rect);
    }
    else if ((piece & bishop) == bishop)
    {
        SDL_RenderCopy(renderer, texture, &tile_src[3 + offset], &selected_rect);
    }
    else if ((piece & knight) == knight)
    {
        SDL_RenderCopy(renderer, texture, &tile_src[2 + offset], &selected_rect);
    }
    else if ((piece & rook) == rook)
    {
        SDL_RenderCopy(renderer, texture, &tile_src[1 + offset], &selected_rect);
    }

    else if ((piece & pawn) == pawn)
    {
        SDL_RenderCopy(renderer, texture, &tile_src[0 + offset], &selected_rect);
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