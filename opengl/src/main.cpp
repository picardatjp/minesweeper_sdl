#include "SDL.h"
#include "SDL_image.h"
#include "glad.h"
#include <iostream>

int main(int argc, char *argv[])
{

    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    SDL_Window *window_;
    bool isRunning_;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // we create the window here
        window_ = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
        SDL_GLContext context;
        context = SDL_GL_CreateContext(window_);
        gladLoadGLLoader(SDL_GL_GetProcAddress);
        if (!window_)
            std::cout << "Failed to create window." << std::endl;

        // we made it here so the game is now running
        isRunning_ = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        isRunning_ = false;
    }

    // game->renderBackground();

    // game loop, stops when Screen::isRunning is false
    while (isRunning_)
    {
        glViewport(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480);
        frameStart = SDL_GetTicks();
        // handle events, then update game objects, then render to the screen
        /**************EVENTS***************/
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
            isRunning_ = false;
            break;
        default:
            break;
        }

        /*************RENDER*************/
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window_);

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_DestroyWindow(window_);
    SDL_Quit();

    return 0;
}