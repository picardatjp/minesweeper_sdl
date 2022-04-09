#include "Screen.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <future>

// SDL_Texture *buttons_texture;
// const char *buttons_texture_path = "res/buttons.png";

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
    // SDL_Surface *temp_surf = IMG_Load(buttons_texture_path);
    // buttons_texture = SDL_CreateTextureFromSurface(renderer, temp_surf);
    // SDL_FreeSurface(temp_surf);

    values_size = 8;
    randomizeValues();
    // values = {4, 3, 2, 7, 6, 5, 7, 1};
    // start the timer here
    change_time_start = change_time_end = SDL_GetTicks();
    // just started, haven't sorted nor recorded the sort
    sorted = false;
    sort_recorded = false;
    swaps_index = 0;
    compares_index = 0;
    // start out with bubble sort
    sort_method = 1;
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
        // button was pressed so we want which ones
        state = SDL_GetKeyboardState(NULL);
        // if "B" was pressed
        if (state[SDL_SCANCODE_B])
        {
            // set sort method to 1==bubble sort
            sort_method = 1;
            // uncheck the sorted bools and clear swaps data
            sorted = sort_recorded = false;
            swaps.clear();
            compares.clear();
            compares_index = 0;
            swaps_index = 0;
            // randomize the values in "values"
            randomizeValues();
        }
        // if "Q" was pressed
        else if (state[SDL_SCANCODE_Q])
        {
            // quick sort
            sort_method = 2;
            sorted = sort_recorded = false;
            swaps.clear();
            compares.clear();
            compares_index = 0;
            swaps_index = 0;
            randomizeValues();
        }
        // if "H" was pressed
        else if (state[SDL_SCANCODE_H])
        {
            // heap sort
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
    // if values is unsorted we need to sort it
    if (!sorted)
        updateGraph();
}

void Screen::updateGraph()
{
    // if we haven't recorded a sort yet
    if (!sort_recorded)
    {
        // bubble sort selected
        if (sort_method == 1)
        {
            // call bubble sort with values
            bubbleSort(values);
            // we recorded the swaps in bubble sort
            sort_recorded = true;
        }
        // quicksort selected
        else if (sort_method == 2)
        {
            // call quicksort with values, start index 0, end index size-1
            quickSort(values, 0, values.size() - 1);
            sort_recorded = true;
        }
        // heap sort selected
        else if (sort_method == 4)
        {
            heapSort(values);
            sort_recorded = true;
        }
        // else if (sort_method == 3)
        // {
        //     mergeSort(values, 0, values.size() - 1);
        //     sort_recorded = true;
        // }
    }
    // update time elapsed since last change
    change_time_end = SDL_GetTicks();
    // if we have waited long enough
    if (change_time_end - change_time_start > (1000 / speed))
    {
        // start the timer over
        change_time_end = change_time_start = SDL_GetTicks();
        // if we still have swaps to update with
        if (swaps_index != swaps.size())
        {
            // we swap the values of "values" using the current recorded swap
            swap(values[swaps[swaps_index].first], values[swaps[swaps_index].second]);
            // next swap is now the current
            swaps_index++;
        }
        else
        {
            // we swapped all the values needed to sort the array so we don't need to call this updateGraph function anymore
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
    // render top part of screen
    renderRibbon();
    // render bar graph
    renderGraph();
    // actually write to the screen
    SDL_RenderPresent(renderer);
}

void Screen::renderRibbon()
{
    // set draw color to grey
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    // make a rect from top left to far right and down 64px, or ribbon_offset
    SDL_Rect r;
    r.x = r.y = 0;
    r.w = getWinWidth();
    r.h = ribbon_offset;
    // draw a filled in rectangle using our rect r
    SDL_RenderFillRect(renderer, &r);
}

void Screen::renderButtions()
{
}

void Screen::renderGraph()
{
    float percent;
    // go through all values
    for (int i = 0; i < values.size(); i++)
    {
        // this is how tall each bar should be as a percent. 1/8 means 1/8 of the screen high-ribbon_offset
        percent = (float)values[i] / (float)values.size();
        // if the index of "values" that we are looking at is being swapped, make the bar red
        if (swaps_index != swaps.size() && (swaps[swaps_index].first == i || swaps[swaps_index].second == i))
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            // otherwise make the bar white
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // draw a line for each pixel of our bar, this could be replaced with SDL_RenderFillRect() now that i think about it. -2 for padding
        for (int j = 0; j < (getWinWidth() / values.size() - 2); j++)
        {
            // draw line starting at bottom of screen up to the percent valueof the screen minus the ribbon_offset at the top
            // SDLRenderDrawLine(renderer,x1,y1,x2,y2)
            SDL_RenderDrawLine(renderer, i * (getWinWidth() / values.size()) + j + 1, getWinHeight(), i * (getWinWidth() / values.size()) + j + 1, int(getWinHeight() - ((getWinHeight() - ribbon_offset) * percent)));
        }
    }
}

void Screen::clean()
{
    // destroy our textures, window, renderer and quit all SDL processes
    // SDL_DestroyTexture(buttons_texture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned." << std::endl;
}

// we pass vector copies so we don't disturb "values"
void Screen::bubbleSort(std::vector<int> v)
{
    // go through every value
    for (int i = 0; i < v.size(); i++)
    {
        // go through every value up to i from the end
        for (int j = 0; j < v.size() - i - 1; j++)
        {
            // if the current value is greater than the one to the right
            if (v[j] > v[j + 1])
            {
                // record the swap
                swaps.push_back(std::pair<int, int>(j, j + 1));
                // swap the current and the one to the right
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
        if (v[0] != v[i])
        {
            swaps.push_back(std::pair<int, int>(0, i));
            swap(v[0], v[i]);
        }
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

// values are by reference so they affect the values out of scope
void Screen::swap(int &a, int &b)
{
    int t = a;
    a = b;
    b = t;
}

void Screen::randomizeValues()
{
    // get random seed for rand
    unsigned int s = time(0);
    srand(s);
    int x;
    // if "values" doesn't have the same size as what we set it to, then set it
    if (values_size != values.size())
        values.resize(values_size, 0);
    // we want to set a new value for every index of "values"
    for (int i = 0; i < values.size(); i++)
    {
        // reset x
        x = 0;
        // we don't want x to be 0, then our bar in the bar graph would have height zero and it would look weird
        while (x < 1)
        {
            // get random number excluding the size of the array. this way we don't have a bar that touches the ribbon, there is a little leeway
            x = rand() % values.size();
        }
        // set value to x
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