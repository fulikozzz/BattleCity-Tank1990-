#pragma once
#include "Wall.h"

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

    unordered_map<WallType, Texture> textures;

public:

    Map();
    
    void loadFromFile(string filePath);
    int getWidth();
    int getHeight();
    Wall& getCell(int x, int y);
    void loadTextures();
    void draw(RenderWindow& window);
};