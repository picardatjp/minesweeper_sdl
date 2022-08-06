#include "Application.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <ctime>

const char *score_path = "../res/score.png";
const char *ng_path = "../res/new_game.png";
const char *bs_path = "../res/board_size.png";

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
        IMG_Init(IMG_INIT_PNG);
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
    board_num_cells_ = 10;
    randomizeBoard();
    curr_color_ = board_[board_.size() - board_num_cells_];
    picked_color_ = -1;
    score_ = 0;
    go_b_ = 30.0f;
    go_g_ = 200.0f;
    go_r_ = 0.0f;
    go_r_v_ = 3.0f;
    go_g_v_ = 2.0f;
    go_b_v_ = 4.0f;

    SDL_Surface *ts = IMG_Load(score_path);
    score_tex_ = SDL_CreateTextureFromSurface(renderer, ts);
    ts = IMG_Load(bs_path);
    bs_tex_ = SDL_CreateTextureFromSurface(renderer, ts);
    ts = IMG_Load(ng_path);
    ng_tex_ = SDL_CreateTextureFromSurface(renderer, ts);
    SDL_FreeSurface(ts);
    // if (ts == nullptr)
    //     std::cout << "problem";
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
            mouse_state = SDL_GetMouseState(&x, &y);
            if (x < GAME_BOARD_DIM_ && y < GAME_BOARD_DIM_)
            {
                picked_color_ = board_[(int)(y / (GAME_BOARD_DIM_ / board_num_cells_)) * board_num_cells_ + (int)(x / (GAME_BOARD_DIM_ / board_num_cells_))];
                if (!checkGameEnd())
                {
                    if (picked_color_ != curr_color_)
                    {
                        std::cout << ++score_ << "\n";
                        // std::cout << board_[((int)(y / (GAME_BOARD_DIM_ / board_num_cells_)) * board_num_cells_ + (int)(x / (GAME_BOARD_DIM_ / board_num_cells_)))] << "\n";
                        floodFill(0, 9);
                        curr_color_ = picked_color_;
                    }
                }
                else
                {
                    std::cout << "game is over. score: " << score_ << "\n";
                }
            }
        }
    default:
        break;
    }
}

// update game objects here eg. sprite locations adn whatnot
void Application::update()
{
    go_b_ += go_b_v_;
    go_r_ += go_r_v_;
    go_g_ += go_g_v_;
    if (go_b_ > 255.0f || go_b_ < 0.0f)
    {
        go_b_v_ *= -1.0f;
        go_b_ += go_b_v_;
    }
    if (go_r_ > 255.0f || go_r_ < 0.0f)
    {
        go_r_v_ *= -1.0f;
        go_r_ += go_r_v_;
    }
    if (go_g_ > 255.0f || go_g_ < 0.0f)
    {
        go_g_v_ *= -1.0f;
        go_g_ += go_g_v_;
    }
}

// our render function which draws to the screen
void Application::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderClear(renderer);
    renderGameBoard();
    renderUI();
    SDL_RenderPresent(renderer);
}

void Application::renderGameBoard()
{
    // start at top left
    SDL_Rect rect = {0, 0, GAME_BOARD_DIM_ / board_num_cells_, GAME_BOARD_DIM_ / board_num_cells_};
    // go through board_ and render rectangles based on the color in board_
    for (int i = 0; i < board_num_cells_; i++)
    {
        // move rect down
        rect.y = GAME_BOARD_DIM_ / board_num_cells_ * i;
        for (int j = 0; j < board_num_cells_; j++)
        {
            // move rect right
            rect.x = GAME_BOARD_DIM_ / board_num_cells_ * j;
            // pick colors
            if (board_[i * board_num_cells_ + j] == 0)
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            else if (board_[i * board_num_cells_ + j] == 1)
                SDL_SetRenderDrawColor(renderer, 24, 127, 24, 255);
            else if (board_[i * board_num_cells_ + j] == 2)
                SDL_SetRenderDrawColor(renderer, 240, 140, 30, 255);
            else if (board_[i * board_num_cells_ + j] == 3)
                SDL_SetRenderDrawColor(renderer, 250, 200, 120, 255);
            else if (board_[i * board_num_cells_ + j] == 4)
                SDL_SetRenderDrawColor(renderer, 250, 120, 200, 255);
            else if (board_[i * board_num_cells_ + j] == 5)
                SDL_SetRenderDrawColor(renderer, 242, 240, 30, 255);
            else if (board_[i * board_num_cells_ + j] == 6)
                SDL_SetRenderDrawColor(renderer, 200, 100, 255, 255);
            else if (board_[i * board_num_cells_ + j] == 7)
                SDL_SetRenderDrawColor(renderer, 100, 255, 50, 255);
            else
                SDL_SetRenderDrawColor(renderer, 50, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Application::renderUI()
{

    SDL_Rect r1 = {100,
                   620,
                   120,
                   60};
    // SDL_SetRenderDrawColor(renderer, go_r_, go_g_, go_b_, 255);
    SDL_RenderFillRect(renderer, &r1);
    // render black border around ui section, there's probably a draw rect with border func idk
    r1.h = 4;
    r1.w = 600;
    r1.x = 0;
    r1.y = 600;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r1);
    r1.y = 696;
    SDL_RenderFillRect(renderer, &r1);
    r1.h = 92;
    r1.w = 4;
    r1.y = 604;
    SDL_RenderFillRect(renderer, &r1);
    r1.x = 596;
    SDL_RenderFillRect(renderer, &r1);

    r1.h = 40;
    r1.w = 150;
    r1.x = 37;
    r1.y = 630;
    // SDL_RenderFillRect(renderer, &r1);
    SDL_RenderCopy(renderer, score_tex_, NULL, &r1);
    r1.x = 224;
    SDL_RenderCopy(renderer, ng_tex_, NULL, &r1);
    r1.x = 411;
    SDL_RenderCopy(renderer, bs_tex_, NULL, &r1);
    SDL_RenderPresent(renderer);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyTexture(bs_tex_);
    SDL_DestroyTexture(score_tex_);
    SDL_DestroyTexture(ng_tex_);
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
}

// flood fill algorithm
// infinite loop when floodFill in won state, so i made checkGameEnd() to stop it
void Application::floodFill(int x, int y)
{
    // if out of bounds, return
    if (x < 0 || x >= board_num_cells_ || y < 0 || y >= board_num_cells_)
        return;
    // if the square we are looking at is the same color as the bottomleft square
    else if (board_[y * board_num_cells_ + x] == curr_color_)
    {
        // we change color and look up and to the right for more same colored squares
        board_[y * board_num_cells_ + x] = picked_color_;
        floodFill(x + 1, y);
        floodFill(x, y - 1);
        floodFill(x - 1, y);
        floodFill(x, y + 1);
    }
    return;
}

bool Application::checkGameEnd()
{
    // go through board_ to see if all the same color
    for (int i : board_)
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
    board_.clear();
    for (int i = 0; i < board_num_cells_ * board_num_cells_; i++)
    {
        while (x == 0)
        {
            x = rand() % 10;
        }
        board_.push_back(x - 1);
        x = 0;
    }
}