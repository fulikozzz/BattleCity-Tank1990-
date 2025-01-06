#pragma once
#include "Wall.h"
#include "Base.h"

#include <string>
#include <array>
#include <unordered_map>

using namespace std;
using namespace sf;

#define STANDARD_HEIGHT 20
#define STANDARD_WIDTH 20

class Map
{
private:
    int width;    
    int height;     
    array<array<Wall, STANDARD_WIDTH>, STANDARD_HEIGHT> grid;
    Base player_base;
    Base enemy_base;

    unordered_map<WallType, Texture> textures;

public:
    Map();
    
    int getWidth();
    int getHeight();
    Base& getPlayerBase();
    Base& getEnemyBase();
    Wall& getCell(int x, int y);
    unordered_map<WallType, Texture>& getMap();
    Texture& getCellTexture(int x, int y);

    void loadFromFile(string filePath);
    void loadTextures();
    void draw(RenderWindow& window);
};