#include "Screen.hpp"
#include "Tetris.hpp"
#include <memory>

SDL_Renderer *Screen::renderer = nullptr;

const char *tileset_path = "res/tiles2.png";
SDL_Texture *tile_texture;
const char *bg_path = "res/mainbg.png";
SDL_Texture *bg_texture;
const char *Abg_path = "res/tetrisA2.png";
SDL_Texture *Abg_texture;
std::unique_ptr<Tetris> tetris = std::make_unique<Tetris>();
SDL_Rect tileset[8];

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
        // we create the window here
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (!window)
            std::cout << "Failed to create window." << std::endl;

        // create our renderer (not sure what the -1 and 0 are)
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer)
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
    tempSurface = IMG_Load(bg_path);
    bg_texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    tempSurface = IMG_Load(Abg_path);
    Abg_texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    tetris->newGame();
    std::cout << tetris->getScore() << "\n";
    for (int i = 0; i < 8; i++)
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
        key_presses++;
        if (state[SDL_SCANCODE_RIGHT])
        {
            if (!tetris->getLost())
                tetris->moveRight();
        }
        if (state[SDL_SCANCODE_LEFT])
        {
            if (!tetris->getLost())
                tetris->moveLeft();
        }
        if (state[SDL_SCANCODE_DOWN])
        {
            if (!tetris->getLost())
                tetris->moveDown(true);
        }
        if (state[SDL_SCANCODE_Z])
        {
            if (!tetris->getLost())
                tetris->rotate(false);
        }
        if (state[SDL_SCANCODE_X])
        {
            if (!tetris->getLost())
                tetris->rotate(true);
        }
        if (state[SDL_SCANCODE_P])
        {
            current_screen = 0;
        }
        if (state[SDL_SCANCODE_SPACE])
        {
            if (current_screen == 0)
            {
                current_screen = 2;
                tetris->newGame();
            }
            else if (current_screen == 2)
            {
                if (!tetris->getLost())
                    tetris->drop();
            }
        }
        break;
    default:
        break;
    }
}

// update game objects here eg. sprite locations adn whatnot
void Screen::update()
{
    if (!tetris->getLost() && current_screen == 2)
        tetris->updateTime();
    // checkLine() whenever just before spawning a new tetrimino!!
}

// our render function which draws to the screen
void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // renderBackground();
    if (current_screen == 0)
    {
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
    }
    else if (current_screen == 2)
    {
        renderBoard();
    }

    // renderCurrentPiece();

    SDL_RenderPresent(renderer);
}

void Screen::renderBoard()
{
    SDL_RenderCopy(renderer, Abg_texture, NULL, NULL);
    SDL_Rect r;
    r.w = r.h = tile_size;
    r.x = r.y = 0;
    for (int i = 0; i < 252; i++)
    {
        r.x = (i % BOARD_TILE_WIDTH) * tile_size + BOARD_X_OFFSET;
        r.y = (int(i / BOARD_TILE_WIDTH)) * tile_size + BOARD_Y_OFFSET;
        if (tetris->getDisplayFieldElement(i) == 0)
            SDL_RenderCopy(renderer, tile_texture, &tileset[0], &r);
        else if (tetris->getDisplayFieldElement(i) == 1)
            SDL_RenderCopy(renderer, tile_texture, &tileset[1], &r);
        else if (tetris->getDisplayFieldElement(i) == 2)
            SDL_RenderCopy(renderer, tile_texture, &tileset[2], &r);
        else if (tetris->getDisplayFieldElement(i) == 3)
            SDL_RenderCopy(renderer, tile_texture, &tileset[3], &r);
        else if (tetris->getDisplayFieldElement(i) == 4)
            SDL_RenderCopy(renderer, tile_texture, &tileset[4], &r);
        else if (tetris->getDisplayFieldElement(i) == 5)
            SDL_RenderCopy(renderer, tile_texture, &tileset[5], &r);
        else if (tetris->getDisplayFieldElement(i) == 6)
            SDL_RenderCopy(renderer, tile_texture, &tileset[6], &r);
        else if (tetris->getDisplayFieldElement(i) == 7)
            SDL_RenderCopy(renderer, tile_texture, &tileset[7], &r);
    }
    Piece p = tetris->getCurrentPiece();
    for (int i = 0; i < 16; i++)
    {
        r.x = (i % 4) * tile_size + p.x_offset * tile_size + BOARD_X_OFFSET;
        r.y = (int(i / 4)) * tile_size + p.y_offset * tile_size + BOARD_Y_OFFSET;
        if (tetris->getPieceElement(p.piece, p.rotation, i) && r.y > 150)
        {
            if (p.piece == 0)
                SDL_RenderCopy(renderer, tile_texture, &tileset[1], &r);
            else if (p.piece == 1)
                SDL_RenderCopy(renderer, tile_texture, &tileset[2], &r);
            else if (p.piece == 2)
                SDL_RenderCopy(renderer, tile_texture, &tileset[3], &r);
            else if (p.piece == 3)
                SDL_RenderCopy(renderer, tile_texture, &tileset[4], &r);
            else if (p.piece == 4)
                SDL_RenderCopy(renderer, tile_texture, &tileset[5], &r);
            else if (p.piece == 5)
                SDL_RenderCopy(renderer, tile_texture, &tileset[6], &r);
            else if (p.piece == 6)
                SDL_RenderCopy(renderer, tile_texture, &tileset[7], &r);
        }
    }
    p = tetris->getNextPiece();
    for (int i = 0; i < 16; i++)
    {
        if (p.piece == 6)
        {
            r.x = (i % 4) * tile_size + p.x_offset * tile_size + NEXT_PIECE_X_OFFSET + 27;
            r.y = (int(i / 4)) * tile_size + p.y_offset * tile_size + NEXT_PIECE_Y_OFFSET;
        }
        else if (p.piece == 0)
        {
            r.x = (i % 4) * tile_size + p.x_offset * tile_size + NEXT_PIECE_X_OFFSET + 13;
            r.y = (int(i / 4)) * tile_size + p.y_offset * tile_size + NEXT_PIECE_Y_OFFSET + 20;
        }
        else if (p.piece == 2)
        {
            r.x = (i % 4) * tile_size + p.x_offset * tile_size + NEXT_PIECE_X_OFFSET + 12;
            r.y = (int(i / 4)) * tile_size + p.y_offset * tile_size + NEXT_PIECE_Y_OFFSET;
        }
        else
        {
            r.x = (i % 4) * tile_size + p.x_offset * tile_size + NEXT_PIECE_X_OFFSET;
            r.y = (int(i / 4)) * tile_size + p.y_offset * tile_size + NEXT_PIECE_Y_OFFSET;
        }
        if (tetris->getPieceElement(p.piece, p.rotation, i))
        {
            if (p.piece == 0)
                SDL_RenderCopy(renderer, tile_texture, &tileset[1], &r);
            else if (p.piece == 1)
                SDL_RenderCopy(renderer, tile_texture, &tileset[2], &r);
            else if (p.piece == 2)
                SDL_RenderCopy(renderer, tile_texture, &tileset[3], &r);
            else if (p.piece == 3)
                SDL_RenderCopy(renderer, tile_texture, &tileset[4], &r);
            else if (p.piece == 4)
                SDL_RenderCopy(renderer, tile_texture, &tileset[5], &r);
            else if (p.piece == 5)
                SDL_RenderCopy(renderer, tile_texture, &tileset[6], &r);
            else if (p.piece == 6)
                SDL_RenderCopy(renderer, tile_texture, &tileset[7], &r);
        }
    }
}

// clean up memory and close things
void Screen::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyTexture(tile_texture);
    SDL_DestroyTexture(Abg_texture);
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Screen cleaned." << std::endl;
}