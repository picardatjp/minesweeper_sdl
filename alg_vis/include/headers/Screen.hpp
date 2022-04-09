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
    // values to be sorted
    std::vector<int> values;
    // number of integers in values
    int values_size;
    // used to calculate how long it has been since the bar graph was updated
    Uint32 change_time_start = 0;
    Uint32 change_time_end = 0;
    // the current index of swaps
    int swaps_index = 0;
    // is values sorted
    bool sorted = false;
    // have the swaps been recorded
    bool sort_recorded = false;
    // stores pairs of indexes that were swapped when the values were sorted
    std::vector<std::pair<int, int>> swaps;
    // stores comparisons, not really using this at the moment
    std::vector<std::pair<int, int>> compares;
    int compares_index = 0;
    // height of the grey ribbon at the top of the screen
    int ribbon_offset = 64;
    // the method being used for sorting :heap, bubble, etc.
    int sort_method = 0;
    // the speed at which the bar graph gets updated. 1000ms/speed.
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
    // updates the bar graph
    void updateGraph();
    // renders the bar graph
    void renderGraph();
    // renders the top part of the screen
    void renderRibbon();
    // renders the buttons
    void renderButtions();
    // randomizes integers in values and sets values size to values_size if they are not already the same
    void randomizeValues();
    // bubble sort implementation
    void bubbleSort(std::vector<int> v);
    // swaps two values in an array
    void swap(int &a, int &b);
    // helper function for quickSort
    int partition(std::vector<int> &v, int low, int high);
    // quicksort implementation
    void quickSort(std::vector<int> v, int low, int high);
    // helper function for mergesort
    void merge(std::vector<int> &v, int left, int middle, int right);
    // mergesort implementation
    void mergeSort(std::vector<int> v, int begin, int end);
    // helper function for heapsort
    void heapify(std::vector<int> &v, int n, int i);
    // heapsort implementation
    void heapSort(std::vector<int> v);
    // a long nop
    void wait();
};

#endif