#include "Application.hpp"

// global game
Application *app = nullptr;

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
    app = new Application();

    // initialize our game
    app->init("paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app->getWinWidth(), app->getWinHeight(), false);

    // frameStart = SDL_GetTicks();
    // game loop
    while (app->running())
    {
        frameStart = SDL_GetTicks();
        // handle events, then update game objects, then render to the screen
        app->handleEvents();
        app->update();
        app->render();

        // sdl_getticks returns milliseconds since program start
        frameTime = SDL_GetTicks() - frameStart;
        // if we haven't used up our 1/60th of a second between frames then we delay until then
        // if (FRAME_DELAY < frameTime)
        // {
        //     app->render();
        //     frameStart = SDL_GetTicks();
        // }
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    // game is no longer running so we clean up
    app->clean();

    return 0;
}