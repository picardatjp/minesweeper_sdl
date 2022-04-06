#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

struct tile
{
    // ID synonymous with vector index
    const int id;
    // team that the tile belongs to, either 1 or 0
    int team;
    // number of dice that are on the tile
    int num_dice;

    bool operator==(const tile &t) const
    {
        return id == t.id;
    }
    bool operator<(const tile &t) const
    {
        return id < t.id;
    }
};

// all the actual logic of the game will happen within this class
class Game
{
private:
    // the number of total tiles that should be on the board
    const int NUM_OF_TILES = 20;
    // starting number of dice for each team
    const int NUM_START_DICE = 30;
    // max number of dice that can be in one tile
    const int MAX_DICE = 8;
    std::vector<tile> tiles;
    std::vector<std::vector<int>> tiles_adj_list;

public:
    Game();
    ~Game();

    // returns true if the attacking tile beat the defending tile
    bool roll(int a_dice, int b_dice);
    // returns true if the attacking tile has the defending tile a neighbor and that they are on opposing teams
    bool canAttack(int a_tile, int b_tile);
    // a_tile attacks b_tile. if a_tile wins then move all but one die to b_tile, and make b_tile team to that of a_tile
    // if a_tile loses then leave a_tile with only one die
    void attack(int a_tile, int b_tile);
    // adds new tile to our game
    void addNewTile(int team, int dice);
    // adds a new edge between tiles, used to see which tiles are touching eachother
    bool addTileEdge(int a_id, int b_id);
    // ends the turn and next player can make their move
    void endTurn(int team);
    // adds dice to a teams tiles
    void addDiceToTiles(int team, int num_dice);
    // sets everything up for a new game. creates a new board with new tiles and such
    void newBoard();

    void clearTileData();
};

#endif