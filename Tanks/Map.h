#pragma once
#include "Wall.h"

#include <string>
#include <array>
#include <unordered_map>
using namespace std;

class Map
{
private:
    int width;          // 20
    int height;         // 20
    array<array<Wall, 20>, 20> grid;

    unordered_map<WallType, sf::Texture> textures;

public:

    Map();
    
    void loadFromFile(std::string filePath);
    int getWidth();
    int getHeight();
    Wall getCell(int x, int y);
    void loadTextures();
    void draw(sf::RenderWindow& window);
};