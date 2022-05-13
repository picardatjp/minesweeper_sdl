#include "Screen.hpp"

// global game
Screen *screen = nullptr;

// main function
int main(int argc, char *argv[])
{

    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    // create new game
    screen = new Screen();

    // initialize our game
    screen->init("my game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 720, false);

    // game->renderBackground();

    // game loop, stops when Screen::isRunning is false
    while (screen->running())
    {
        frameStart = SDL_GetTicks();
        // handle events, then update game objects, then render to the screen
        screen->handleEvents();
        screen->update();
        screen->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    // game is no longer running so we clean up
    screen->clean();

    return 0;
}