#include "Application.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>

std::random_device rd;
const char *pillar_path = "../res/pillar.png";
const char *gameover_path = "../res/gameover.png";
const char *numbers_path = "../res/numbers.png";
const char *bird_path = "../res/bird.png";

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
        IMG_Init(IMG_INIT_PNG);
        // create our renderer (not sure what the -1 and 0 are)
        renderer_ = SDL_CreateRenderer(window_, -1, 0);
        if (!renderer_)
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
    bird_rect_ = {100, 100, 70, 40};
    SDL_Rect r = {WINDOW_WIDTH_, 0, pillar_width_, 100};
    for (int i = 0; i < 3; i++)
    {
        // rect = { x y w h }
        pillar_rects_.push_back(r);
        pillar_rects_.push_back(r);
        randomizePillarY(i * 2);
        r.x += WINDOW_WIDTH_ / 3 + pillar_width_ / 3;
    }
    SDL_Surface *ts = IMG_Load(pillar_path);
    pillar_tex_ = SDL_CreateTextureFromSurface(renderer_, ts);
    SDL_FreeSurface(ts);
    ts = IMG_Load(gameover_path);
    gameover_tex_ = SDL_CreateTextureFromSurface(renderer_, ts);
    SDL_FreeSurface(ts);
    ts = IMG_Load(numbers_path);
    numbers_tex_ = SDL_CreateTextureFromSurface(renderer_, ts);
    SDL_FreeSurface(ts);
    ts = IMG_Load(bird_path);
    bird_tex_ = SDL_CreateTextureFromSurface(renderer_, ts);
    SDL_FreeSurface(ts);
    score_ = pillar_rects_.size() / -2;
}

// this is where events like input are handled
void Application::handleEvents()
{
    // create an event
    SDL_Event event;
    const Uint8 *state = NULL;
    // poll the event
    SDL_PollEvent(&event);
    // find what events happened
    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !space_down_)
        {
            space_down_ = true;
        }
        if (!gamestart_)
            gamestart_ = true;
        break;

    case SDL_KEYUP:
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            space_down_ = false;
            jump_reset_ = true;
        }
        break;
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running_ = false;
        break;
    default:
        break;
    }
}

// update game objects here eg. sprite locations and whatnot
void Application::update()
{
    // update positions and states here

    if (gamestart_)
    {
        if (!gameover_)
        {
            if (space_down_ && jump_reset_)
            {
                y_v_ = -10.0f;
                jump_reset_ = false;
            }

            checkPillarCollision();
            checkPillarBounds();
            for (int i = 0; i < pillar_rects_.size(); i++)
            {
                pillar_rects_[i].x -= 3;
            }
        }
        else
        {

            if (space_down_ && SDL_GetTicks() - d_start_ > 500)
            {
                gameover_ = false;
                y_v_ = -10.0f;
                jump_reset_ = false;
                bird_rect_.y = WINDOW_HEIGHT_ / 2 - bird_rect_.h;
                int x = WINDOW_WIDTH_;
                for (int i = 0; i < 3; i++)
                {
                    pillar_rects_[i * 2].x = x;
                    pillar_rects_[i * 2 + 1].x = x;
                    randomizePillarY(i * 2);
                    x += WINDOW_WIDTH_ / 3 + pillar_width_ / 3;
                }
                score_ = pillar_rects_.size() / -2;
            }
        }
        y_v_ += .6f;
        bird_rect_.y += (int)y_v_;
        if (bird_rect_.y >= WINDOW_HEIGHT_ - bird_rect_.h)
        {
            bird_rect_.y = WINDOW_HEIGHT_ - bird_rect_.h;
        }
        if (bird_rect_.y <= 0)
        {
            bird_rect_.y = 1;
            y_v_ = 0.0f;
        }
    }
}

// our render function which draws to the screen
void Application::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer_, 60, 180, 230, 0);
    SDL_RenderClear(renderer_);

    // render stuff here

    // SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 0);
    SDL_Rect r = {0, 0, pillar_width_, 0};
    for (int i = 0; i < 3; i++)
    {
        r.y = 900 - pillar_rects_[i * 2].h;
        r.h = pillar_rects_[i * 2].h;
        SDL_RenderCopy(renderer_, pillar_tex_, &r, &pillar_rects_[i * 2]);
        r.y = 0;
        r.h = pillar_rects_[i * 2 + 1].h;
        SDL_RenderCopy(renderer_, pillar_tex_, &r, &pillar_rects_[i * 2 + 1]);
    }
    SDL_RenderCopy(renderer_, bird_tex_, NULL, &bird_rect_);
    if (gameover_)
    {
        // render gameover stuff
        displayEndScreen();
    }
    displayScore();
    SDL_RenderPresent(renderer_);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyTexture(pillar_tex_);
    SDL_DestroyTexture(gameover_tex_);
    IMG_Quit();
    SDL_Quit();
}

void Application::checkPillarBounds()
{
    for (int i = 0; i < pillar_rects_.size(); i += 2)
    {
        if (pillar_rects_[i].x + pillar_width_ <= 0)
        {
            randomizePillarY(i);
            pillar_rects_[i].x = WINDOW_WIDTH_;
            pillar_rects_[i + 1].x = WINDOW_WIDTH_;
        }
    }
}

// only randomizes the height of the gap
void Application::randomizePillarY(int p)
{
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> randomize(0, WINDOW_HEIGHT_ - pillar_gap_ - 80);

    int new_height = randomize(rd) + 40;
    pillar_rects_[p].h = new_height;
    pillar_rects_[p + 1].y = pillar_rects_[p].h + pillar_gap_;
    pillar_rects_[p + 1].h = WINDOW_HEIGHT_ - pillar_rects_[p + 1].y;
    score_++;
}

void Application::checkPillarCollision()
{
    for (SDL_Rect p : pillar_rects_)
    {
        if (bird_rect_.y < p.h + p.y && bird_rect_.y + bird_rect_.h > p.y)
        {
            if (bird_rect_.x < p.x + p.w && bird_rect_.x + bird_rect_.w > p.x)
            {
                gameover_ = true;
                getScores();
                d_start_ = SDL_GetTicks();
            }
        }
    }
}

void Application::getScores()
{
    std::ifstream myFile("../res/scores.txt");
    int num = 0;
    if (myFile.is_open())
    {
        myFile >> num;
        scores_[0] = num;
        myFile >> num;
        scores_[1] = num;
        myFile >> num;
        scores_[2] = num;
    }
    if (score_ > scores_[0])
    {
        scores_[2] = scores_[1];
        scores_[1] = scores_[0];
        scores_[0] = score_;
    }
    else if (score_ > scores_[1])
    {
        scores_[2] = scores_[1];
        scores_[1] = score_;
    }
    else if (score_ > scores_[2])
    {
        scores_[2] = score_;
    }
    myFile.close();
    std::ofstream myfile("../res/scores.txt", std::ios::trunc);
    for (int i = 0; i < 3; i++)
    {
        myfile << scores_[i];
        myfile << " ";
    }
    myfile.close();
}

void Application::displayEndScreen()
{
    SDL_Rect r = {300, 100, 300, 150};
    SDL_Rect r2 = {0, 0, 32, 32};
    SDL_RenderCopy(renderer_, gameover_tex_, NULL, &r);
    r.y = 250;
    r.w = r.h = 32;
    for (int i = 0; i < 3; i++)
    {
        r2.x = ((int)scores_[i] / 100) * 32;
        r.x = 400;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
        r2.x = ((int)(scores_[i] / 10) % 10) * 32;
        r.x += 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
        r2.x = ((int)(scores_[i]) % 10) * 32;
        r.x += 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
        r.y += 50;
    }
}

void Application::displayScore()
{
    SDL_Rect r = {0, 0, 32, 32};
    SDL_Rect r2 = {0, 0, 32, 32};
    if (score_ > 99)
    {
        r2.x = ((int)score_ / 100) * 32;
        r.x = WINDOW_WIDTH_ - 32 - 32 - 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
        r2.x = ((int)(score_ / 10) % 10) * 32;
        r.x += 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
        r2.x = ((int)(score_) % 10) * 32;
        r.x += 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
    }
    else if (score_ > 9)
    {
        r2.x = ((int)(score_ / 10) % 10) * 32;
        r.x = WINDOW_WIDTH_ - 32 - 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
        r2.x = ((int)(score_) % 10) * 32;
        r.x += 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
    }
    else if (score_ > 0)
    {
        r2.x = ((int)(score_) % 10) * 32;
        r.x = WINDOW_WIDTH_ - 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
    }
    else
    {
        r2.x = 0;
        r.x = WINDOW_WIDTH_ - 32;
        SDL_RenderCopy(renderer_, numbers_tex_, &r2, &r);
    }
}