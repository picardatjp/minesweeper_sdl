/******************************************************************************
 *
 *   Project: notepad
 *   Author: Jacob Picardat
 *   Date:   4/24/2022
 *   Purpose:    A simple notepad clone. Doing stuff like this
 *               in SDL is pretty much from scratch so I am just messing around
 *               with this one. Not really sure the best way to go about doing
 *               UI stuff such as buttons and textboxes so I am just winging it.
 *
 ******************************************************************************/

#include "Screen.hpp"

// global game
Screen *screen = nullptr;

// main function
int main(int argc, char *argv[])
{
    // our frames per second set at 60
    const int FPS = 60;
    // 1/60th of a second == 1000ms/60
    const int FRAME_DELAY = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    // create new game
    screen = new Screen();

    // initialize our game
    screen->init("notepad", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen->getWinWidth(), screen->getWinHeight(), false);

    // game loop, stops when Game::isRunning is false
    while (screen->running())
    {
        // set timer start
        frameStart = SDL_GetTicks();
        // handle events, then update game objects, then render to the screen
        screen->handleEvents();
        screen->update();
        screen->render();

        // sdl_getticks returns milliseconds since program start
        frameTime = SDL_GetTicks() - frameStart;
        // if we haven't used up our 1/60th of a second between frames then we delay until then
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    // game is no longer running so we clean up
    screen->clean();

    return 0;
}