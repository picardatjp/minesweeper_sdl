#include "Game.h"

// our static renderer
SDL_Renderer *Game::renderer = nullptr;
// path to tilesheet
const char *tileset_path = "res/mtiles.png";
// our texture
SDL_Texture *tile_texture;
// array of source rects
SDL_Rect tileset[12];

// these store mouse click location
int mouse_x, mouse_y;
Uint32 button;
// number of flags placed on the board
int flag_count = 0;
// win and lose variables
bool won;
bool lost;

// board size and cell pixel size
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 10;
const int CELL_HEIGHT = 32;
const int CELL_WIDTH = 32;
// number of bombs in the level
const int NUM_BOMBS = 8;
// board_d is the board description, basically the board fully uncovered
int board_d[BOARD_HEIGHT][BOARD_WIDTH];
// board_r is the board to be rendered
int board_r[BOARD_HEIGHT][BOARD_WIDTH];

// Game Constructor
Game::Game()
{
}

// Game Destructor
Game::~Game()
{
}

// Game initializer function
void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
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
            // set the screen to black
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
    // create a surface, create texture from that surface, free the surface
    SDL_Surface *tempSurface = IMG_Load(tileset_path);
    tile_texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    // set source rects to capture each part of our tilesheet. there are 12 32x32 tiles all laid our horizontally in our texture
    // (x,y) is start of rect, (w,h) is end of rect, so each source rect has one tile picture thing associated with it
    // 0 = empty cell, 1-8 = 1-8, 9 = covered cell, 10 = flagged cell, 11 = bomb eg. tileset[7] == tile from our texture that is a cell with the number 7 on it
    for (int i = 0; i < 12; i++)
    {
        tileset[i].y = 0;
        tileset[i].x = i * CELL_WIDTH;
        tileset[i].w = CELL_WIDTH;
        tileset[i].h = CELL_HEIGHT;
    }
    // create a new board cause the program just started
    newBoard();
}

// this is where events like input are handled
void Game::handleEvents()
{
    // create an event
    SDL_Event event;
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
    // user released a mouse button
    case SDL_MOUSEBUTTONUP:
        // get the x and y of where they released it
        button = SDL_GetMouseState(&mouse_x, &mouse_y);
        // get what cell was clicked
        mouse_x = floor(mouse_x / 32);
        mouse_y = floor(mouse_y / 32);
        // left click means we uncover cells
        if (event.button.button == SDL_BUTTON_LEFT && !lost)
        {
            // if cell clicked was covered
            if (board_r[mouse_y][mouse_x] == 9)
            {
                // we uncover all the cells neighboring the one clicked
                uncoverCells(mouse_x, mouse_y);
            }
        }
        // right click means we flag cells, don't want to change flags if you already won
        else if (event.button.button == SDL_BUTTON_RIGHT && !won && !lost)
        {
            // if cell is covered
            if (board_r[mouse_y][mouse_x] == 9)
            {
                // put a flag on it and increase our flag count
                board_r[mouse_y][mouse_x] = 10;
                flag_count++;
                // if we have the same number of flags as bombs we can check to see if the user won
                if (flag_count == NUM_BOMBS && checkWin())
                {
                    // win animation here
                    std::cout << "you won" << std::endl;
                    won = true;
                }
            }
            // if the user clicked on a flag
            else if (board_r[mouse_y][mouse_x] == 10)
            {
                // take the flag off the cell and decrement our counter
                board_r[mouse_y][mouse_x] = 9;
                flag_count--;
                // we can check if the user won
                if (flag_count == NUM_BOMBS && checkWin())
                {
                    // win animation here
                    std::cout << "you won" << std::endl;
                    won = true;
                }
            }
        }
        break;
    // no event happened
    default:
        break;
    }
}

// update game objects here eg. sprite locations adn whatnot
void Game::update()
{
    // i dont even use this, why did i make this
}

// our render function which draws to the screen
void Game::render()
{
    // clears the screen
    SDL_RenderClear(renderer);
    // render our board
    renderBoard();
    //  actually write to the screen
    SDL_RenderPresent(renderer);
}

// clean up memory and close SDL
void Game::clean()
{
    // destroy our spritesheet texture, window, renderer and quit all SDL processes
    SDL_DestroyTexture(tile_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

// displays the board to the screen using the board_r array
void renderBoard()
{
    // set destination rect to size 32x32
    SDL_Rect r;
    r.w = r.h = 32;
    // go through every cell in board_r
    for (int i = 0; i < 10; i++)
    {
        // every cell we draw is CELL_HEIGHT x CELL_WIDTH so we offset by those values
        r.y = i * 32;
        for (int j = 0; j < 10; j++)
        {
            r.x = j * 32;
            // display our offset cell based on the value in board_r
            SDL_RenderCopy(Game::renderer, tile_texture, &tileset[board_r[i][j]], &r);
        }
    }
}

// reset the board for a new game, populates the board_d array
void newBoard()
{
    won = false;
    lost = false;
    // set board_d to empty cells and board_r to covered cells
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            board_d[i][j] = 0;
            board_r[i][j] = 9;
        }
    }
    // get a count of how many bombs need to be put in the board
    int bombs_left = NUM_BOMBS;
    int x, y;
    // seed the rand()
    srand((unsigned)time(0));
    // while we still have bombs to place
    while (bombs_left > 0)
    {
        // get random coordinates to place a bomb at
        x = rand() % 10;
        y = rand() % 10;
        // if there isn't already a bomb here
        if (board_d[y][x] != 11)
        {
            // we set this cell to a bomb
            board_d[y][x] = 11;
            bombs_left--;
            // we increment all surrounding cells cause they all have a bomb next to them
            // check if in bounds and not that the cell isn't also a bomb
            if (y > 0 && x > 0 && board_d[y - 1][x - 1] != 11)
            {
                board_d[y - 1][x - 1]++;
            }
            if (x > 0 && board_d[y][x - 1] != 11)
            {
                board_d[y][x - 1]++;
            }
            if (y < BOARD_HEIGHT - 1 && x > 0 && board_d[y + 1][x - 1] != 11)
            {
                board_d[y + 1][x - 1]++;
            }
            if (y > 0 && board_d[y - 1][x] != 11)
            {
                board_d[y - 1][x]++;
            }
            if (y < BOARD_HEIGHT - 1 && board_d[y + 1][x] != 11)
            {
                board_d[y + 1][x]++;
            }
            if (y > 0 && x < BOARD_WIDTH - 1 && board_d[y - 1][x + 1] != 11)
            {
                board_d[y - 1][x + 1]++;
            }
            if (x < BOARD_WIDTH - 1 && board_d[y][x + 1] != 11)
            {
                board_d[y][x + 1]++;
            }
            if (y < BOARD_HEIGHT - 1 && x < BOARD_WIDTH - 1 && board_d[y + 1][x + 1] != 11)
            {
                board_d[y + 1][x + 1]++;
            }
        }
    }
}

// flood fill for uncovering cells when the user clicks on the board
void uncoverCells(int x, int y)
{
    // if out of bounds, return
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
        return;
    // if we hit a bomb
    if (board_d[y][x] == 11)
    {
        // we lost :P
        std::cout << "you lost" << std::endl;
        lost = true;
        // do losing stuff here
        return;
    }
    //***********************************************************************
    // there is a bug in here, not sure what is happening. whenever I put if(board_r[y][x] != 9 && board_r[y][x] == 10) it breaks :(
    // if cell is already uncovered or flagged, return
    if (board_r[y][x] != 9 && true)
        return;
    if (board_r[y][x] == 10)
        return;
    //***********************************************************************
    // if cell is numbered, uncover it and return
    if (board_d[y][x] > 0 && board_d[y][x] < 9)
    {
        board_r[y][x] = board_d[y][x];
        return;
    }
    // here means the cell is empty so we recurse over all neighboring cells
    board_r[y][x] = board_d[y][x];
    uncoverCells(x - 1, y - 1);
    uncoverCells(x, y - 1);
    uncoverCells(x + 1, y - 1);
    uncoverCells(x - 1, y);
    uncoverCells(x + 1, y);
    uncoverCells(x - 1, y + 1);
    uncoverCells(x, y + 1);
    uncoverCells(x + 1, y + 1);
}

// check to see if the player won
bool checkWin()
{
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            // if the cell has a bomb not covered by a flag
            if (board_d[i][j] == 11 && board_r[i][j] != 10)
                return false;
        }
    }
    // all bombs had flags on them, you won!
    return true;
}