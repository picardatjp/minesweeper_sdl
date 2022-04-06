#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>

struct tile
{
    // ID synonymous with vector index
    const int ID;
    int team;
    int numOfDice;

    bool operator==(const tile &t) const
    {
        return ID == t.ID;
    }
    bool operator<(const tile &t) const
    {
        return ID < t.ID;
    }
};
void addEdge(std::map<tile, std::vector<tile>> &m, tile &a, tile &b)
{
    m[a].push_back(b);
    m[b].push_back(a);
}
bool addTileEdge(std::vector<std::vector<int>> &v, int aID, int bID)
{
    if (std::find(v[aID].begin(), v[aID].end(), bID) == v[aID].end())
    {
        v[aID].push_back(bID);
        v[bID].push_back(aID);
        return true;
    }

    return false;
}
void addNewTile(std::vector<tile> &v, int team, int dice)
{
    tile t = {(int)v.size(), team, dice};
    v.push_back(t);
}
int main()
{
    // std::map<tile, std::vector<tile>> myMap;
    // tile a;
    // a.numOfDice = 6;
    // a.team = 1;
    // a.ID = 0;
    // tile b;
    // b.numOfDice = 5;
    // b.team = 1;
    // b.ID = 1;
    // addEdge(myMap, a, b);
    // b.numOfDice = 2;
    // b.team = 0;
    // b.ID = 2;
    // addEdge(myMap, a, b);
    // for (std::pair<tile, std::vector<tile>> e : myMap)
    // {
    //     tile t1 = e.first;
    //     std::cout << t1.ID << " -> ";
    //     for (int i = 0; i < e.second.size(); i++)
    //     {
    //         tile t2 = e.second[i];
    //         std::cout << t2.ID << ", ";
    //     }
    //     std::cout << std::endl;
    // }
    int n = 10;
    std::vector<tile> v1;
    std::vector<std::vector<int>> v1_al(n);
    for (int i = 0; i < n; i++)
    {
        addNewTile(v1, i % 2, 3);
    }
    addTileEdge(v1_al, v1[0].ID, v1[4].ID);
    addTileEdge(v1_al, v1[2].ID, v1[4].ID);
    addTileEdge(v1_al, v1[2].ID, v1[7].ID);
    addTileEdge(v1_al, v1[7].ID, v1[8].ID);
    addTileEdge(v1_al, v1[8].ID, v1[6].ID);
    addTileEdge(v1_al, v1[8].ID, v1[3].ID);
    addTileEdge(v1_al, v1[3].ID, v1[1].ID);
    addTileEdge(v1_al, v1[5].ID, v1[3].ID);
    addTileEdge(v1_al, v1[9].ID, v1[3].ID);
    addTileEdge(v1_al, v1[6].ID, v1[9].ID);

    // for (int i = 0; i < v1_al.size(); i++)
    // {
    //     std::cout << v1[i].ID << " -> ";
    //     for (int j = 0; j < v1_al[i].size(); j++)
    //     {
    //         std::cout << v1_al[i][j] << ", ";
    //     }
    //     std::cout << std::endl;
    // }
    return 0;
}
