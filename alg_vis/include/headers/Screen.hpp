#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "SDL.h"
#include <vector>
#include <random>
// #include <algorithm>

// our screen class that handles basically everything
class Screen
{
private:
    int count = 0;

    // true if the game is running, false if not
    bool is_running;
    // our game window
    SDL_Window *window;
    SDL_Renderer *renderer;
    // window width and height
    int win_height = 480;
    int win_width = 640;
    std::vector<int> values;
    int values_size;
    int update_time = 0;
    Uint32 change_time_start = 0;
    Uint32 change_time_end = 0;
    int swaps_index = 0;
    bool sorted = false;
    bool sort_recorded = false;
    std::vector<std::pair<int, int>> swaps;
    std::vector<std::pair<int, int>> compares;
    int compares_index = 0;
    int ribbon_offset = 64;
    int sort_method = 0;
    int speed = 4;

public:
    // constructor and destructor
    Screen();
    ~Screen();

    // init declaration, this starts up the game
    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

    // handles events
    void handleEvents();
    // updates game objects
    void update();
    // refreshes screen with updated info
    void render();
    // cleans game memory and stops SDL
    void clean();

    // returns whether the game is currently running or not
    bool running() { return is_running; }
    // getter and setter for window height and width
    int getWinHeight() { return win_height; }
    int getWinWidth() { return win_width; }
    void setWinHeight(int h) { win_height = h; }
    void setWinWidth(int w) { win_width = w; }

    void updateGraph();
    void renderGraph();
    void renderRibbon();
    void renderButtions();
    int getUpdateTime() { return update_time; }
    void setUpdateTime();
    void randomizeValues();

    void bubbleSort(std::vector<int> v);
    void swap(int &a, int &b);
    int partition(std::vector<int> &v, int low, int high);
    void quickSort(std::vector<int> v, int low, int high);
    void merge(std::vector<int> &v, int left, int middle, int right);
    void mergeSort(std::vector<int> v, int begin, int end);
    void heapify(std::vector<int> &v, int n, int i);
    void heapSort(std::vector<int> v);
    void wait();

    // our screen renderer

    // static std::random_device rd;
    // std::default_random_engine rng = std::default_random_engine{rd()};
};

#endif