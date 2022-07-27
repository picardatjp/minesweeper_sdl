#include "Application.hpp"
#include "SDL.h"
#include <iostream>
#include <ctime>

SDL_Renderer *Application::renderer = nullptr;

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
        renderer = SDL_CreateRenderer(window_, -1, 0);
        if (!renderer)
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
    board_dim_ = 10;
    randomizeBoard();
    curr_color_ = board[board.size() - board_dim_];
    picked_color_ = curr_color_;
    score_ = 0;
}

// this is where events like input are handled
void Application::handleEvents()
{
    // create an event
    SDL_Event event;
    Uint32 mouse_state;
    int x, y;
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
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            if (!checkGameEnd())
            {
                std::cout << ++score_ << "\n";
                mouse_state = SDL_GetMouseState(&x, &y);
                picked_color_ = board[(int)(y / (WINDOW_HEIGHT_ / board_dim_)) * board_dim_ + (int)(x / (WINDOW_WIDTH_ / board_dim_))];
                // std::cout << board[((int)(y / (WINDOW_HEIGHT_ / board_dim_)) * board_dim_ + (int)(x / (WINDOW_WIDTH_ / board_dim_)))] << "\n";
                floodFill(0, 9);
                curr_color_ = picked_color_;
            }
            else
            {
                std::cout << "game is over. score: " << score_ << "\n";
            }
        }
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    // start at top left
    SDL_Rect rect = {0, 0, WINDOW_WIDTH_ / board_dim_, WINDOW_HEIGHT_ / board_dim_};
    // go through board and render rectangles based on the color in board
    for (int i = 0; i < board_dim_; i++)
    {
        // move rect down
        rect.y = WINDOW_HEIGHT_ / board_dim_ * i;
        for (int j = 0; j < board_dim_; j++)
        {
            // move rect right
            rect.x = WINDOW_WIDTH_ / board_dim_ * j;
            // pick colors
            if (board[i * board_dim_ + j] == 0)
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            else if (board[i * board_dim_ + j] == 1)
                SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
            else if (board[i * board_dim_ + j] == 2)
                SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
            else if (board[i * board_dim_ + j] == 3)
                SDL_SetRenderDrawColor(renderer, 250, 200, 120, 255);
            else if (board[i * board_dim_ + j] == 4)
                SDL_SetRenderDrawColor(renderer, 250, 120, 200, 255);
            else if (board[i * board_dim_ + j] == 5)
                SDL_SetRenderDrawColor(renderer, 200, 120, 250, 255);
            else if (board[i * board_dim_ + j] == 6)
                SDL_SetRenderDrawColor(renderer, 200, 250, 100, 255);
            else if (board[i * board_dim_ + j] == 7)
                SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
            else if (board[i * board_dim_ + j] == 8)
                SDL_SetRenderDrawColor(renderer, 100, 255, 50, 255);
            else
                SDL_SetRenderDrawColor(renderer, 50, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// flood fill algorithm
// infinite loop when floodFill in won state, so i made checkGameEnd() to stop it
void Application::floodFill(int x, int y)
{
    // if out of bounds, return
    if (x < 0 || x >= board_dim_ || y < 0 || y >= board_dim_)
        return;
    // if the square we are looking at is the same color as the bottomleft square
    else if (board[y * board_dim_ + x] == curr_color_)
    {
        // we change color and look up and to the right for more same colored squares
        board[y * board_dim_ + x] = picked_color_;
        floodFill(x + 1, y);
        floodFill(x, y - 1);
        floodFill(x - 1, y);
        floodFill(x, y + 1);
    }
    return;
}

bool Application::checkGameEnd()
{
    // go through board to see if all the same color
    for (int i : board)
    {
        if (curr_color_ != i)
            return false;
    }
    return true;
}

void Application::randomizeBoard()
{
    int x = 0;
    srand((unsigned)time(0));
    board.clear();
    for (int i = 0; i < board_dim_ * board_dim_; i++)
    {
        while (x == 0)
        {
            x = rand() % 11;
        }
        board.push_back(x - 1);
        x = 0;
    }
}