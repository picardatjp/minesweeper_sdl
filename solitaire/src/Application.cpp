#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "Application.hpp"
#include "Card.hpp"
#include <vector>
#include <random>

std::random_device rd;
std::vector<Card> deck;
std::vector<Card> stack;
const char *cards_path = "../res/cards.png";

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
    SDL_Surface *ts = IMG_Load(cards_path);
    cards_tex_ = SDL_CreateTextureFromSurface(renderer_, ts);
    SDL_FreeSurface(ts);
    std::vector<int> deck;
    shuffleDeck();
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
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running_ = false;
        break;
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
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);

    // render stuff here
    // drawCards();
    SDL_Rect s = {0, 0, 75, 105};
    SDL_Rect d = {200, 200, 75, 105};
    SDL_RenderCopy(renderer_, cards_tex_, &s, &d);
    SDL_RenderPresent(renderer_);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyTexture(cards_tex_);
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}

void Application::drawCards()
{
    SDL_Rect src;
    src.w = CARD_WIDTH_;
    src.h = CARD_HEIGHT_;
    for (int i = 0; i < deck.size(); i++)
    {
        src.x = deck[i].getNumber() * CARD_WIDTH_;
        src.y = deck[i].getSuit() * CARD_HEIGHT_;
        SDL_RenderCopy(renderer_, cards_tex_, &src, deck.at(i).getDestRect());
    }
}

void Application::shuffleDeck()
{
    //{ x y w h }
    SDL_Rect r = {100, 100, CARD_WIDTH_, CARD_HEIGHT_};
    for (int i = 0; i < deck.size(); i++)
    {
        deck[i].setSuit(getRand(0, 3));
        deck[i].setNumber(getRand(0, 13));
        deck[i].setDestRect(r);
    }
}

void Application::populateDeck()
{
    for (int i = 0; i < 52; i++)
    {
        Card c;
        c.setSuit(getRand(0, 3));
        c.setNumber(getRand(0, 13));
        deck.push_back(c);
    }
}

int Application::getRand(int l, int u)
{
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> randomize(l, u);
    return randomize(rd);
}