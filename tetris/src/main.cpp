#include "Game.hpp"

// global game
Game *game = nullptr;

// main function
int main(int argc, char *argv[])
{

    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    // create new game
    game = new Game();

    // initialize our game
    game->init("my game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

    // game->renderBackground();

    // game loop, stops when Game::isRunning is false
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        // handle events, then update game objects, then render to the screen
        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    // game is no longer running so we clean up
    game->clean();

    return 0;
}