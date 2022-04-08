#include "Screen.hpp"

// our static renderer
SDL_Renderer *Screen::renderer = nullptr;

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
        if (renderer)
        {
            // set the screen to black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            // std::cout << "Renderer created successfully." << std::endl;
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
    // 2d representation of our world map
    worldMap =
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 1},
            {1, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 1},
            {1, 0, 4, 0, 4, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 0, 3, 0, 1},
            {1, 0, 4, 0, 4, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 3, 3, 0, 1},
            {1, 0, 4, 0, 4, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 0, 0, 0, 2, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1},
            {1, 0, 4, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
    // resize screen buffer
    std::vector<Uint32> temp_vec(getWinWidth());
    buffer.resize(getWinHeight(), temp_vec);

    // create texture buffer
    textureBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, getWinWidth(), getWinHeight());
}

void Screen::handleEvents()
{
    // create an event
    SDL_Event event;
    // poll the event
    SDL_PollEvent(&event);
    // stores keyboard button presses
    const Uint8 *state;
    // find what events happened
    switch (event.type)
    {
    // the window was closed
    case SDL_QUIT:
        // so we stop running the game
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W])
            w_pressed = true;
        if (state[SDL_SCANCODE_S])
            s_pressed = true;
        if (state[SDL_SCANCODE_A])
            a_pressed = true;
        if (state[SDL_SCANCODE_D])
            d_pressed = true;
        break;
    case SDL_KEYUP:
        state = SDL_GetKeyboardState(NULL);
        if (!state[SDL_SCANCODE_W])
            w_pressed = false;
        if (!state[SDL_SCANCODE_S])
            s_pressed = false;
        if (!state[SDL_SCANCODE_A])
            a_pressed = false;
        if (!state[SDL_SCANCODE_D])
            d_pressed = false;
    // no event happened
    default:
        break;
    }
}

void Screen::update()
{
    if (w_pressed)
    {
        if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false)
            posX += dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false)
            posY += dirY * moveSpeed;
    }
    if (s_pressed)
    {
        if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false)
            posX -= dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false)
            posY -= dirY * moveSpeed;
    }
    if (d_pressed)
    {
        oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    if (a_pressed)
    {
        oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
}

void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderWorld();
    //  actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderWorld()
{

    // Uint32 *pPixelBuffer = nullptr;
    for (int x = 0; x < getWinWidth(); x++)
    {
        float cameraX = 2 * x / float(getWinWidth()) - 1;
        float rayDirX = dirX + planeX * cameraX;
        float rayDirY = dirY + planeY * cameraX;

        int mapX = int(posX);
        int mapY = int(posY);

        float sideDistX, sideDistY;

        float deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
        float perpWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }
        int dof = 0;
        while (hit == 0 && dof < 100)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (worldMap[mapX][mapY] > 0)
                hit = 1;
            dof++;
        }

        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        int lineHeight = (int)(getWinHeight() / perpWallDist);

        int drawStart = -lineHeight / 2 + getWinHeight() / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + getWinHeight() / 2;
        if (drawEnd >= getWinHeight())
            drawEnd = getWinHeight() - 1;

        //////////////////////////// this bit is for hopefully drawing textures soon, i haven't figured out a good way to do it yet with sdl
        // int texNum = worldMap[mapX][mapY] - 1;
        // float wallX;
        // if (side == 0)
        //     wallX = posY + perpWallDist * rayDirY;
        // else
        //     wallX = posX + perpWallDist * rayDirX;
        // wallX -= floor(wallX);

        // int texX = int(wallX * float(texWidth));
        // if (side == 0 && rayDirX > 0)
        //     texX = texWidth - texX - 1;
        // if (side == 1 && rayDirY < 0)
        //     texX = texWidth - texX - 1;

        // float step = 1.0 * texHeight / lineHeight;
        // float texPos = (drawStart - getWinHeight() / 2 + lineHeight / 2) * step;

        // for (int y = drawStart; y < drawEnd; y++)
        // {
        //     int texY = (int)texPos & (texHeight - 1);
        //     texPos += step;
        //     Uint32 color = texture[texNum][texHeight * texY + texX];
        //     if (side == 1)
        //         color = (color >> 1) & 8355711;
        //     buffer[y][x] = color;
        // }

        // int32_t pitch = 0;
        // SDL_LockTexture(texture, NULL, (void **)&pPixelBuffer, &pitch);
        // pitch /= sizeof(uint32_t);
        // for (int y = drawStart; y < drawEnd; y++)
        // {
        //     int texY = (int)texPos & (texHeight - 1);
        //     texPos += step;
        //     pPixelBuffer[(x * getWinHeight()) + y] = (255 << 24) | (red << 16) | (green << 8) | blue;
        // }
        // SDL_UnlockTexture(texture);

        //////////// return (alpha << 24) | (red << 16) | (green << 8) | blue;
        /////////////////////////////////////

        switch (worldMap[mapX][mapY])
        {
        case 1:
            if (side == 1)
                SDL_SetRenderDrawColor(renderer, 127, 0, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            break;
        case 2:
            if (side == 1)
                SDL_SetRenderDrawColor(renderer, 0, 127, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            break;
        case 3:
            if (side == 1)
                SDL_SetRenderDrawColor(renderer, 0, 0, 127, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        case 4:
            if (side == 1)
                SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;
        default:
            if (side == 1)
                SDL_SetRenderDrawColor(renderer, 127, 127, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            break;
        }
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
    // SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}
