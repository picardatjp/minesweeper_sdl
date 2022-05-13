#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <random>
#include <ctime>
#include <set>
#include <string>

std::string picture_path = "res/pic1.png";
std::string buttons_path = "res/puzzle_buttons.png";
std::string play_button_path = "res/play_button.png";
std::string bg_path = "res/bg.png";

Screen::Screen()
{
}

Screen::~Screen()
{
}

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
        // std::cout << "SDL initialized successfully." << std::endl;

        // we create the window here
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (!window)
            std::cout << "Failed to create window." << std::endl;

        // create our renderer (not sure what the -1 and 0 are)
        renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (renderer)
        {
            // set the screen to black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            // std::cout << "Renderer created successfully." << std::endl;
        }
        else
            std::cout << "Failed to create renderer." << std::endl;

        // we made it here so the game is now running
        is_running = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        is_running = false;
    }
    // load up our button textures
    SDL_Surface *temp = IMG_Load(buttons_path.c_str());
    buttons_texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    temp = IMG_Load(play_button_path.c_str());
    play_button_texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    temp = IMG_Load(bg_path.c_str());
    bg_texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    // initialize button rects to some magic numbers
    buttons_dest.x = button_highlight_dest.x = 17;
    buttons_dest.y = button_highlight_dest.y = 245;
    buttons_dest.h = buttons_src.h = button_highlight_src.h = button_highlight_dest.h = button_highlight_dest.w = button_highlight_src.w = 150;
    play_button_dest.h = play_button_dest.w = play_button_src.h = play_button_src.w = 150;
    buttons_dest.w = buttons_src.w = 606;
    buttons_src.x = buttons_src.y = button_highlight_src.y = play_button_src.y = play_button_src.x = 0;
    button_highlight_src.x = 606;
    play_button_dest.x = 245;
    play_button_dest.y = 442;
    // set initial pic selected to the first one
    pic_selected = 0;
    SDL_Rect r;
    for (int i = 0; i < 16; i++)
    {
        r.x = (i % 4) * 160;
        r.y = int(i / 4) * 160;
        r.w = r.h = 160;
        tiles[i].src = r;
        tiles[i].dest = r;
        tiles[i].id = i;
    }
    newBoard();
    // board = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
}

void Screen::handleEvents()
{
    // create an event
    SDL_Event event;
    // poll the event
    SDL_PollEvent(&event);
    // stores keyboard button presses
    const Uint8 *state;
    int x, y;
    // find what events happened

    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        is_running = false;
        break;
    case SDL_KEYDOWN:
        // button was pressed so we want which ones
        state = SDL_GetKeyboardState(NULL);
        // if "B" was pressed
        if (state[SDL_SCANCODE_Q])
        {
            // newBoard();
            // sorted = false;
            current_screen = 0;
        }
        // if (state[SDL_SCANCODE_1] && pic_selected != 0)
        // {
        //     loadTexture(0);
        //     newBoard();
        //     sorted = false;
        //     pic_selected = 0;
        // }
        // if (state[SDL_SCANCODE_2] && pic_selected != 1)
        // {
        //     loadTexture(1);
        //     newBoard();
        //     sorted = false;
        //     pic_selected = 1;
        // }
        // if (state[SDL_SCANCODE_3] && pic_selected != 2)
        // {
        //     loadTexture(2);
        //     newBoard();
        //     sorted = false;
        //     pic_selected = 2;
        // }
        // if (state[SDL_SCANCODE_4] && pic_selected != 3)
        // {
        //     loadTexture(3);
        //     newBoard();
        //     sorted = false;
        //     pic_selected = 3;
        // }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            if (current_screen == 0)
            {
                SDL_GetMouseState(&x, &y);
                if (y > 245 && y < 395)
                {
                    // pic 0 selected (button 0)
                    if (x > 17 && x < 167)
                    {
                        button_highlight_dest.x = 17;
                        pic_selected = 0;
                    }
                    // pic 1 selected (button1)
                    if (x > 169 && x < 319)
                    {
                        button_highlight_dest.x = 169;
                        pic_selected = 1;
                    }
                    // pic 2 selected (button2)
                    if (x > 321 && x < 471)
                    {
                        button_highlight_dest.x = 321;
                        pic_selected = 2;
                    }
                    // pic 3 selected (button3)
                    if (x > 473 && x < 623)
                    {
                        button_highlight_dest.x = 473;
                        pic_selected = 3;
                    }
                }
                if (y > 442 && y < 592 && x > 245 && x < 395)
                {
                    playClicked();
                }
            }
            if (current_screen == 1)
            {
                SDL_GetMouseState(&x, &y);
                mouseDown = true;
                initMX = x;
                initMY = y;
                initMX_offset = x - (int(x / 160)) * 160;
                initMY_offset = y - (int(y / 160)) * 160;
            }
        }
        break;
    // no event happened
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            // std::cout << "released ";
            if (current_screen == 1)
            {
                mouseDown = false;
                snapTile();
                checkWin();
            }
        }
        break;
    default:
        break;
    }
}

void Screen::update()
{
    // if we have our mouse held down we want to move a tile
    if (mouseDown)
    {
        moveTile();
    }
}

void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    SDL_RenderClear(renderer);
    // render board or main menu
    if (current_screen == 0)
        renderMenu();
    if (current_screen == 1)
        renderBoard();
    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::moveTile()
{
    // get the x and y position of the mouse
    int x, y;
    SDL_GetMouseState(&x, &y);
    //
    int tx = initMX / 160;
    int ty = initMY / 160;
    int selected_tile = ty * 4 + tx;
    if (selected_tile > 3 && tiles[selected_tile - 4].id == 0)
    {
        // if the selected tile is below the empty tile
        if (y - initMY_offset < tiles[selected_tile - 4].dest.y)
        {
            tiles[selected_tile].dest.y = tiles[selected_tile - 4].dest.y;
        }
        else if (y > initMY)
        {
            tiles[selected_tile].dest.y = initMY - initMY_offset;
        }
        else
        {
            tiles[selected_tile].dest.y = y - initMY_offset;
        }
        tiles[selected_tile].dest.x = initMX - initMX_offset;
    }
    else if (selected_tile < 12 && tiles[selected_tile + 4].id == 0)
    {
        if (y - initMY_offset > tiles[selected_tile + 4].dest.y)
        {
            tiles[selected_tile].dest.y = tiles[selected_tile + 4].dest.y;
        }
        else if (y < initMY)
        {
            tiles[selected_tile].dest.y = initMY - initMY_offset;
        }
        else
        {
            tiles[selected_tile].dest.y = y - initMY_offset;
        }
        tiles[selected_tile].dest.x = initMX - initMX_offset;
    }
    else if (selected_tile % 4 > 0 && tiles[selected_tile - 1].id == 0)
    {
        if (x - initMX_offset < tiles[selected_tile - 1].dest.x)
        {
            tiles[selected_tile].dest.x = tiles[selected_tile - 1].dest.x;
        }
        else if (x > initMX)
        {
            tiles[selected_tile].dest.x = initMX - initMX_offset;
        }
        else
        {
            tiles[selected_tile].dest.x = x - initMX_offset;
        }
        tiles[selected_tile].dest.y = initMY - initMY_offset;
    }
    else if (selected_tile % 4 < 3 && tiles[selected_tile + 1].id == 0)
    {
        if (x - initMX_offset > tiles[selected_tile + 1].dest.x)
        {
            tiles[selected_tile].dest.x = tiles[selected_tile + 1].dest.x;
        }
        else if (x < initMX)
        {
            tiles[selected_tile].dest.x = initMX - initMX_offset;
        }
        else
        {
            tiles[selected_tile].dest.x = x - initMX_offset;
        }
        tiles[selected_tile].dest.y = initMY - initMY_offset;
    }
}

void Screen::snapTile()
{
    int tx = initMX / 160;
    int ty = initMY / 160;
    int selected_tile = ty * 4 + tx;
    int snapToX = (int((tiles[selected_tile].dest.x + 80) / 160));
    int snapToY = (int((tiles[selected_tile].dest.y + 80) / 160));
    tiles[selected_tile].dest.x = (int(initMX / 160)) * 160;
    tiles[selected_tile].dest.y = (int(initMY / 160)) * 160;
    Tile t = tiles[selected_tile];
    tiles[selected_tile] = tiles[snapToY * 4 + snapToX];
    tiles[snapToY * 4 + snapToX] = t;
    SDL_Rect r;
    r = tiles[selected_tile].dest;
    tiles[selected_tile].dest = tiles[snapToY * 4 + snapToX].dest;
    tiles[snapToY * 4 + snapToX].dest = r;
}

void Screen::renderBoard()
{
    // go through all tiles
    for (int i = 0; i < 16; i++)
    {
        // draw every tile except the first one
        if (tiles[i].id != 0)
            SDL_RenderCopy(renderer, texture, &tiles[i].src, &tiles[i].dest);
    }
}

void Screen::newBoard()
{
    std::set<int> s;
    Tile temp[16];
    int x;
    bool found = false;
    unsigned int seed = time(0);
    srand(seed);
    for (int i = 0; i < 16; i++)
    {
        found = false;
        while (!found)
        {
            x = 0;
            while (x < 1)
                x = rand() % 17;
            if (s.insert(x - 1).second)
                found = true;
        }
        temp[i].src = tiles[x - 1].src;
        temp[i].id = tiles[x - 1].id;
    }
    for (int i = 0; i < 16; i++)
    {
        tiles[i].src = temp[i].src;
        tiles[i].id = temp[i].id;
    }
    if (!isSolvable())
    {
        Tile t = tiles[0];
        tiles[0] = tiles[1];
        tiles[1] = t;
        SDL_Rect r = tiles[0].dest;
        tiles[0].dest = tiles[1].dest;
        tiles[1].dest = r;
    }
}

// i don't think this works properly, only 80% of the time i think
bool Screen::isSolvable()
{
    int ic = 0;
    int zero_loc = 0;
    for (int i = 0; i < 15; i++)
    {
        for (int j = i + 1; j < 16; j++)
        {
            if (tiles[i].id != 0 && tiles[j].id != 0 && tiles[i].id > tiles[j].id)
            {
                ic++;
            }
            if (tiles[j].id == 0)
                zero_loc = j;
        }
    }
    if ((ic % 2 == 0 && (int(zero_loc / 4)) % 2 == 0) || (ic % 2 == 1 && (int(zero_loc / 4)) % 2 == 1))
        return true;
    else
        return false;
}

void Screen::checkWin()
{
    bool b = true;
    for (int i = 0; i < 16; i++)
    {
        if (tiles[i].id != i)
            b = false;
    }
    if (b)
    {
        sorted = true;
        std::cout << "sorted!" << std::endl;
    }
}

void Screen::loadTexture(int pic)
{
    switch (pic)
    {
    case 1:
        picture_path = "res/pic1.png";
        break;
    case 2:
        picture_path = "res/pic2.png";
        break;
    case 3:
        picture_path = "res/pic3.png";
        break;
    default:
        picture_path = "res/pic0.png";
        break;
    }
    SDL_DestroyTexture(texture);
    SDL_Surface *temp_surf = IMG_Load(picture_path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, temp_surf);
    SDL_FreeSurface(temp_surf);
}

void Screen::renderMenu()
{
    // render background
    SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
    // render button pictures
    SDL_RenderCopy(renderer, buttons_texture, &buttons_src, &buttons_dest);
    SDL_RenderCopy(renderer, play_button_texture, &play_button_src, &play_button_dest);
    // render highlight
    SDL_RenderCopy(renderer, buttons_texture, &button_highlight_src, &button_highlight_dest);
}

void Screen::playClicked()
{
    loadTexture(pic_selected);
    sorted = false;
    newBoard();
    current_screen = 1;
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(buttons_texture);
    SDL_DestroyTexture(play_button_texture);
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}