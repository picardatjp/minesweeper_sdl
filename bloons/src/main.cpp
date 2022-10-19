#include "Application.hpp"

// global game
Application *app = nullptr;

// main function
int main(int argc, char *argv[])
{

    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    // create new game
    app = new Application();

    // initialize our game
    app->init("bloons", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app->getWinWidth(), app->getWinHeight(), false);

    // game loop, stops when Application::is_running_ is false
    while (app->running())
    {
        frameStart = SDL_GetTicks();
        // handle events, then update game objects, then render to the screen
        app->handleEvents();
        app->update();
        app->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    // game is no longer running so we clean up
    app->clean();

    return 0;
}