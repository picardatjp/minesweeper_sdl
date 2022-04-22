#include "Screen.hpp"

SDL_Renderer *Screen::renderer = nullptr;

const char *tileset_path = "res/tiles.png";
SDL_Texture *tile_texture;

SDL_Rect tileset[12];

// Screen Constructor
Screen::Screen()
{
}

// Screen Destructor
Screen::~Screen()
{
}

// Screen initializer function
void Screen::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
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
        std::cout << "SDL initialized successfully." << std::endl;

        // we create the window here
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window)
            std::cout << "Window created successfully." << std::endl;
        else
            std::cout << "Failed to create window." << std::endl;

        // create our renderer (not sure what the -1 and 0 are)
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            // set the screen to white
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created successfully." << std::endl;
        }
        else
            std::cout << "Failed to create renderer." << std::endl;

        // we made it here so the game is now running
        isRunning = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        isRunning = false;
    }

    // instantiate stuff here

    SDL_Surface *tempSurface = IMG_Load(tileset_path);
    tile_texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    for (int i = 0; i < 12; i++)
    {
        tileset[i].y = 0;
        tileset[i].x = i * 32;
        tileset[i].w = 32;
        tileset[i].h = 32;
    }
}

// this is where events like input are handled
void Screen::handleEvents()
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
        isRunning = false;
        break;
    // no event happened
    case SDL_KEYDOWN:
        state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_RIGHT])
        {
        }
        if (state[SDL_SCANCODE_LEFT])
        {
        }
        if (state[SDL_SCANCODE_DOWN])
        {
        }
        // Tetrimino::moveLeft();
        break;
    case SDL_KeyCode::SDLK_d:
        // Tetrimino::moveRight();
        break;
    case SDL_KeyCode::SDLK_q:
        // Tetrimino::rotateCCW();
        break;
    case SDL_KeyCode::SDLK_e:
        // Tetrimino::rotateCW();
        break;
    case SDL_KeyCode::SDLK_SPACE:
        // Tetrimino::drop();
        break;
    default:
        break;
    }
}

// update game objects here eg. sprite locations adn whatnot
void Screen::update()
{
    // update stuff here

    // checkLine() whenever just before spawning a new tetrimino!!
}

// our render function which draws to the screen
void Screen::render()
{
    // clears the screen
    SDL_RenderClear(renderer);

    // draw stuff between clear() and present()
    // renderTetrimino();
    //  actually write to the screen
    SDL_RenderPresent(renderer);
}

// clean up memory and close things
void Screen::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyTexture(tile_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Screen cleaned." << std::endl;
}