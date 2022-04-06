#include "Game.hpp"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <ctime>

Game::Game()
{
    // set the vector to all empty vectors so we don't try to access an element that doesn't exist yet
    tiles_adj_list.resize(NUM_OF_TILES);
}
Game::~Game()
{
}

bool Game::roll(int a_dice, int b_dice)
{
    // get seed for rand()
    unsigned int s = time(0);
    srand(s);
    // sum of each players roll
    int a_sum = 0;
    int b_sum = 0;
    int x = 0;
    // we want a winner, not a draw
    while (a_sum == b_sum)
    {
        // set to zero at the beginning incase there is a draw and we redo this process
        a_sum = b_sum = 0;
        // go through each die
        for (int i = 0; i < a_dice; i++)
        {
            // using mod is biased to 0 so we don't want to include it
            // also there is no 0 on a die
            while (x < 1)
            {
                // get a random int 1-6
                x = rand() % 7;
            }
            // add the roll to our running total
            a_sum += x;
            // set x to zero so we can do it again
            x = 0;
        }
        // same thing as before but for the defending player
        for (int i = 0; i < b_dice; i++)
        {
            while (x < 1)
            {
                x = rand() % 7;
            }
            b_sum += x;
            x = 0;
        }
    }
    // we return true if the attacker won, false if the attacker lost the exchange
    return a_sum > b_sum;
}

bool ::Game::canAttack(int a, int b)
{
    // if a has neighbor b and a.team != b.team then we can attack
    if (std::find(tiles_adj_list[a].begin(), tiles_adj_list[a].end(), b) != tiles_adj_list[a].end())
        if (tiles[a].team != tiles[b].team)
            return true;
    // otherwise we can't
    return false;
}

void Game::attack(int a_tile, int b_tile)
{
    // if a_tile is able to attack the b_tile
    if (canAttack(a_tile, b_tile))
    {
        // we see who won the exchange
        if (roll(tiles[a_tile].num_dice, tiles[b_tile].num_dice))
        {
            // a_tile won
            // change b_tile's team to that of a_tile
            tiles[b_tile].team = tiles[a_tile].team;
            // move all but one die to b_tile
            tiles[b_tile].num_dice = tiles[a_tile].num_dice - 1;
            // leave the attacking tile with 1 die
            tiles[a_tile].num_dice = 1;
        }
        else
        {
            // a lost
            // dock a_tile down to one die
            tiles[a_tile].num_dice = 1;
        }
    }
}

bool Game::addTileEdge(int a_id, int b_id)
{
    // if the edge doesn't exist
    if (std::find(tiles_adj_list[a_id].begin(), tiles_adj_list[a_id].end(), b_id) == tiles_adj_list[a_id].end())
    {
        // add edge in both directions
        tiles_adj_list[a_id].push_back(b_id);
        tiles_adj_list[b_id].push_back(a_id);
        // we successfully added the edge
        return true;
    }
    // didn't add edge because it already exists
    return false;
}

void Game::addNewTile(int team, int dice)
{
    // create a new tile
    tile t = {(int)tiles.size(), team, dice};
    // add it to the vector passed
    tiles.push_back(t);
}

void Game::clearTileData()
{
    // set the first vector to size=0, and set all the elements of the second vector to an empty vector
    tiles.clear();
    std::vector<int> v;
    std::fill(tiles_adj_list.begin(), tiles_adj_list.end(), v);
}

void Game::endTurn(int team)
{
}

void Game::addDiceToTiles(int team, int num_dice)
{
    // get seed for rand()
    unsigned int s = time(0);
    srand(s);
    // to store all the tiles of the team we are looking at
    std::vector<tile> v_t;
    // go through all tiles
    for (tile t : tiles)
    {
        // if the tile is the same as our team we add it to our vector
        if (t.team = team)
            v_t.push_back(t);
    }
    int x;
    for (int i = 0; i < num_dice; i++)
    {
        // set x to zero so we go into the while loop
        x = 0;
        // rand() % n is biased to 0 so we exclude it
        while (x < 1)
        {
            // get random number 1-v_t.size()
            x = rand() % (v_t.size() + 1);
        }
        // x-1 to bring zero back
        // x-1 is our index of the tile that we increase the dice on
        v_t[x - 1].num_dice++;
    }
}

void Game::newBoard()
{
}