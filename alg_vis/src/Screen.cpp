#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <future>
// our static renderer
// SDL_Renderer *Screen::renderer = nullptr;

SDL_Texture *buttons_texture;
const char *buttons_texture_path = "res/buttons.png";

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
        is_running = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        is_running = false;
    }
    SDL_Surface *temp_surf = IMG_Load(buttons_texture_path);
    buttons_texture = SDL_CreateTextureFromSurface(renderer, temp_surf);
    SDL_FreeSurface(temp_surf);

    values_size = 8;
    // randomizeValues();
    values = {4, 3, 2, 7, 6, 5, 7, 1};
    change_time_start = change_time_end = SDL_GetTicks();
    sorted = false;
    sort_recorded = false;
    swaps_index = 0;
    compares_index = 0;
    sort_method = 4;
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
        is_running = false;
        break;
    case SDL_KEYDOWN:
        state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_B])
        {
            // std::cout << "here";
            sort_method = 1;
            sorted = sort_recorded = false;
            swaps.clear();
            compares.clear();
            compares_index = 0;
            swaps_index = 0;
            randomizeValues();
        }
        else if (state[SDL_SCANCODE_Q])
        {
            sort_method = 2;
            sorted = sort_recorded = false;
            swaps.clear();
            compares.clear();
            compares_index = 0;
            swaps_index = 0;
            randomizeValues();
        }
        else if (state[SDL_SCANCODE_H])
        {
            sort_method = 4;
            sorted = sort_recorded = false;
            swaps.clear();
            compares.clear();
            compares_index = 0;
            swaps_index = 0;
            randomizeValues();
        }
        // if (state[SDL_SCANCODE_M])
        // {
        //     sort_method = 3;
        //     sorted = sort_recorded = false;
        //     swaps.clear();
        //     compares.clear();
        //     compares_index = 0;
        //     swaps_index = 0;
        //     randomizeValues();
        // }
        break;
    // no event happened
    default:
        break;
    }
}

void Screen::update()
{
    if (!sorted)
        updateGraph();
}

void Screen::updateGraph()
{
    if (!sort_recorded)
    {
        if (sort_method == 1)
        {
            bubbleSort(values);
            // auto f = std::async(std::launch::async, &Screen::bubbleSort, this);
            sort_recorded = true;
            // std::cout << swaps[swaps.size() - 1].first << " " << swaps[swaps.size() - 1].second;
        }
        else if (sort_method == 2)
        {
            quickSort(values, 0, values.size() - 1);
            sort_recorded = true;
        }
        else if (sort_method == 4)
        {
            heapSort(values);
            std::cout << "\n\n";
            for (int i : values)
                std::cout << i << ", ";
            sort_recorded = true;
        }
        // else if (sort_method == 3)
        // {
        //     mergeSort(values, 0, values.size() - 1);
        //     sort_recorded = true;
        // }
    }
    change_time_end = SDL_GetTicks();
    if (change_time_end - change_time_start > (1000 / speed))
    {
        change_time_end = change_time_start = SDL_GetTicks();
        if (!(swaps_index == swaps.size()))
        {
            swap(values[swaps[swaps_index].first], values[swaps[swaps_index].second]);
            swaps_index++;
        }
        else
        {
            sorted = true;
            std::cout << "sorted!" << std::endl;
        }
    }
}

void Screen::render()
{
    // clears the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderRibbon();
    renderGraph();
    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderRibbon()
{
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    SDL_Rect r;
    r.x = r.y = 0;
    r.w = getWinWidth();
    r.h = ribbon_offset;
    SDL_RenderFillRect(renderer, &r);
}

void Screen::renderButtions()
{
}

void Screen::renderGraph()
{
    float percent;
    for (int i = 0; i < values.size(); i++)
    {
        percent = (float)values[i] / (float)values.size();
        if (swaps_index != swaps.size() && (swaps[swaps_index].first == i || swaps[swaps_index].second == i))
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int j = 0; j < (getWinWidth() / values.size() - 2); j++)
        {
            SDL_RenderDrawLine(renderer, i * (getWinWidth() / values.size()) + j + 1, getWinHeight(), i * (getWinWidth() / values.size()) + j + 1, int(getWinHeight() - ((getWinHeight() - ribbon_offset) * percent)));
        }
    }
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    SDL_DestroyTexture(buttons_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

// this way we don't change the actual array, just record the changes it makes
void Screen::bubbleSort(std::vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.size() - i - 1; j++)
        {
            if (v[j] > v[j + 1])
            {
                swaps.push_back(std::pair<int, int>(j, j + 1));
                swap(v[j], v[j + 1]);
            }
        }
    }
}

int Screen::partition(std::vector<int> &v, int left, int right)
{
    int pivot = v[right];
    int c = (left - 1);

    for (int i = left; i < right; i++)
    {
        if (v[i] <= pivot)
        {
            c++;
            if (c != i)
                swaps.push_back(std::pair<int, int>(c, i));
            swap(v[c], v[i]);
        }
    }
    if ((c + 1) != right)
        swaps.push_back(std::pair<int, int>(c + 1, right));
    swap(v[c + 1], v[right]);
    return (c + 1);
}

void Screen::quickSort(std::vector<int> v, int left, int right)
{
    if (left < right)
    {
        int p = partition(v, left, right);
        quickSort(v, left, p - 1);
        quickSort(v, p + 1, right);
    }
}

void Screen::heapify(std::vector<int> &v, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    // std::cout << left << " " << right << std::endl;
    if (left < n && v[left] > v[largest])
        largest = left;
    if (right < n && v[right] > v[largest])
        largest = right;
    if (largest != i)
    {
        swaps.push_back(std::pair<int, int>(i, largest));
        swap(v[i], v[largest]);
        heapify(v, n, largest);
    }
}

void Screen::heapSort(std::vector<int> v)
{
    for (int i = v.size() / 2 - 1; i >= 0; i--)
        heapify(v, v.size(), i);
    for (int i = v.size() - 1; i > 0; i--)
    {
        swaps.push_back(std::pair<int, int>(0, i));
        swap(v[0], v[i]);
        heapify(v, i, 0);
    }
}

// void Screen::merge(std::vector<int> &v, int left, int middle, int right)
// {
//     int subArrayOne = middle - left + 1;
//     int subArrayTwo = right - middle;

//     std::vector<int> leftArray(subArrayOne);
//     std::vector<int> rightArray(subArrayTwo);

//     for (int i = 0; i < subArrayOne; i++)
//         leftArray[i] = v[left + i];
//     for (int i = 0; i < subArrayTwo; i++)
//         rightArray[i] = v[middle + 1 + i];

//     int indexOfSubArrayOne = 0;
//     int indexOfSubArrayTwo = 0;
//     int indexOfMergedArray = left;

//     while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
//     {
//         if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo])
//         {
//             swaps.push_back(std::pair<int, int>(indexOfMergedArray, left + indexOfSubArrayOne));
//             v[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
//             indexOfSubArrayOne++;
//         }
//         else
//         {
//             swaps.push_back(std::pair<int, int>(indexOfMergedArray, middle + 1 + indexOfSubArrayTwo));
//             v[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
//             indexOfSubArrayTwo++;
//         }
//     }
//     while (indexOfSubArrayOne < subArrayOne)
//     {
//         swaps.push_back(std::pair<int, int>(indexOfMergedArray, left + indexOfSubArrayTwo));
//         v[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
//         indexOfSubArrayOne++;
//         indexOfMergedArray++;
//     }
//     while (indexOfSubArrayTwo < subArrayTwo)
//     {
//         swaps.push_back(std::pair<int, int>(indexOfMergedArray, middle + 1 + indexOfSubArrayTwo));
//         v[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
//         indexOfSubArrayTwo++;
//         indexOfMergedArray++;
//     }
// }

// void Screen::mergeSort(std::vector<int> v, int begin, int end)
// {
//     if (begin >= end)
//         return;
//     int mid = begin + (end - begin) / 2;
//     mergeSort(v, begin, mid);
//     mergeSort(v, mid + 1, end);
//     merge(v, begin, mid, end);
// }

void Screen::swap(int &a, int &b)
{
    int t = a;
    a = b;
    b = t;
}

void Screen::randomizeValues()
{
    unsigned int s = time(0);
    srand(s);
    int x;
    if (values_size != values.size())
        values.resize(values_size, 0);
    for (int i = 0; i < values.size(); i++)
    {
        x = 0;
        while (x < 1)
        {
            x = rand() % values.size();
        }
        values[i] = x;
    }
}

void Screen::wait()
{
    int end, start;
    end = start = SDL_GetTicks();
    while (end - start < speed)
    {
        end = SDL_GetTicks();
    }
}