#include "Screen.hpp"
#include "ppmTexture.hpp"

SDL_Surface *screen_surface;

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
            {2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, 3, 3, 3, 0, 1},
            {2, 2, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 3, 0, 0, 3, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 3, 0, 0, 3, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 3, 0, 3, 3, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 4, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
    ceilingMap =
        {
            {5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {5, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}};
    floorMap =
        {
            {5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}};
    screen_surface = SDL_GetWindowSurface(window);
    get_ppm_texture(textures[0], "res/tex1.ppm");
    get_ppm_texture(textures[1], "res/tex2.ppm");
    get_ppm_texture(textures[2], "res/tex6.ppm");
    get_ppm_texture(textures[3], "res/tex7.ppm");
    get_ppm_texture(textures[4], "res/tex5.ppm");
    get_ppm_texture(textures[5], "res/tex8.ppm");
    get_ppm_texture(textures[6], "res/tex9.ppm");
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
        if (state[SDL_SCANCODE_E])
            e_pressed = true;
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
        if (state[SDL_SCANCODE_E])
            e_pressed = false;
    // no event happened
    default:
        break;
    }
}

void Screen::update()
{
    // there is bug when walking into a outward corner, i think it's due to the fact that im checking 4 distances from the wall,
    // not a circle around me meaning diagonal movement can get closer to wall and by that point youre too close and stuck
    // ill fix it another day
    if (w_pressed)
    {
        if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false && worldMap[int(posX + dirX * moveSpeed - 0.2f)][int(posY)] == false && worldMap[int(posX + dirX * moveSpeed + 0.2f)][int(posY)] == false)
            posX += dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false && worldMap[int(posX)][int(posY + dirY * moveSpeed - 0.2f)] == false && worldMap[int(posX)][int(posY + dirY * moveSpeed + 0.2f)] == false)
            posY += dirY * moveSpeed;
    }
    if (s_pressed)
    {
        if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false && worldMap[int(posX - dirX * moveSpeed - 0.2f)][int(posY)] == false && worldMap[int(posX - dirX * moveSpeed + 0.2f)][int(posY)] == false)
            posX -= dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false && worldMap[int(posX)][int(posY - dirY * moveSpeed - 0.2f)] == false && worldMap[int(posX)][int(posY - dirY * moveSpeed + 0.2f)] == false)
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
    if (e_pressed)
    {
    }
}

void Screen::render()
{
    SDL_FillRect(screen_surface, NULL, 0);
    renderWorld();
    SDL_UpdateWindowSurface(window);
}

void Screen::renderWorld()
{

    SDL_LockSurface(screen_surface);
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

        int texNum = worldMap[mapX][mapY] - 1;
        float wallX;
        if (side == 0)
            wallX = posY + perpWallDist * rayDirY;
        else
            wallX = posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        int texX = int(wallX * float(texWidth));
        if (side == 0 && rayDirX > 0)
            texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0)
            texX = texWidth - texX - 1;

        float step = 1.0 * texHeight / lineHeight;
        float texPos = (drawStart - getWinHeight() / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++)
        {
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;
            uint8_t r, g, b;
            r = textures[texNum][3 * (texWidth * texY + texX)];
            g = textures[texNum][3 * (texWidth * texY + texX) + 1];
            b = textures[texNum][3 * (texWidth * texY + texX) + 2];
            if (side == 1)
            {
                r *= 0.5f;
                b *= 0.5f;
                g *= 0.5f;
            }
            Uint8 *pixel = (Uint8 *)screen_surface->pixels + y * screen_surface->pitch + x * screen_surface->format->BytesPerPixel;
            *pixel = b;
            pixel++;
            *pixel = g;
            pixel++;
            *pixel = r;
            pixel++;
            *pixel = 255;
        }
        float floorXWall, floorYWall;
        // 4 different wall directions possible
        if (side == 0 && rayDirX > 0)
        {
            floorXWall = mapX;
            floorYWall = mapY + wallX;
        }
        else if (side == 0 && rayDirX < 0)
        {
            floorXWall = mapX + 1.0;
            floorYWall = mapY + wallX;
        }
        else if (side == 1 && rayDirY > 0)
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY;
        }
        else
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY + 1.0;
        }

        float distWall, distPlayer, currentDist;

        distWall = perpWallDist;
        distPlayer = 0.0;

        if (drawEnd < 0)
            drawEnd = getWinHeight();
        for (int y = drawEnd; y < getWinHeight(); y++)
        {
            currentDist = getWinHeight() / (2.0 * y - getWinHeight()); // you could make a small lookup table for this instead

            float weight = (currentDist - distPlayer) / (distWall - distPlayer);

            float currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
            float currentFloorY = weight * floorYWall + (1.0 - weight) * posY;
            if (currentFloorX >= 0 && currentFloorY >= 0 && currentFloorX <= 24 && currentFloorY <= 24)
            {
                int floorTexX, floorTexY;
                floorTexX = int(currentFloorX * texWidth) % texWidth;
                floorTexY = int(currentFloorY * texHeight) % texHeight;
                // questionable
                texNum = floorMap[(int)currentFloorX][(int)currentFloorY] - 1;
                uint8_t r, g, b;
                Uint8 *pixel;
                if (texNum >= 0)
                {
                    r = textures[texNum][3 * (texWidth * floorTexY + floorTexX)] / 2;
                    g = textures[texNum][3 * (texWidth * floorTexY + floorTexX) + 1] / 2;
                    b = textures[texNum][3 * (texWidth * floorTexY + floorTexX) + 2] / 2;
                    pixel = (Uint8 *)screen_surface->pixels + y * screen_surface->pitch + x * screen_surface->format->BytesPerPixel;
                    *pixel = b;
                    pixel++;
                    *pixel = g;
                    pixel++;
                    *pixel = r;
                    pixel++;
                    *pixel = 255;
                }

                texNum = ceilingMap[(int)currentFloorX][(int)currentFloorY] - 1;
                if (texNum >= 0)
                {
                    r = textures[texNum][3 * (texWidth * floorTexY + floorTexX)] / 2;
                    g = textures[texNum][3 * (texWidth * floorTexY + floorTexX) + 1] / 2;
                    b = textures[texNum][3 * (texWidth * floorTexY + floorTexX) + 2] / 2;
                    pixel = (Uint8 *)screen_surface->pixels + (getWinHeight() - y) * screen_surface->pitch + x * screen_surface->format->BytesPerPixel;
                    *pixel = b;
                    pixel++;
                    *pixel = g;
                    pixel++;
                    *pixel = r;
                    pixel++;
                    *pixel = 255;
                }
            }
        }
        SDL_UnlockSurface(screen_surface);
    }
}
void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}
