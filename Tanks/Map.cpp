#include "Map.h"
#include <stdexcept>
#include <fstream>

Map::Map() {
    width = STANDARD_WIDTH;
    height = STANDARD_HEIGHT;
    for (auto& elementsA : grid) {
        for (auto& elements : elementsA) {
            elements = Wall(Position(0, 0), Empty);
        }
    }
    loadTextures();
}

void Map::loadFromFile(string filePath) {
    ifstream in(filePath);

    if (!in.is_open()) {
        throw runtime_error("Ошибка: невозможно открыть файл карты!");
    }

    try {
        for (int y = 0; y < height; ++y) {
            string line;
            if (!getline(in, line) || line.size() != width) {
                throw runtime_error("Ошибка: файл карты не соответствует формату!");
            }

            for (int x = 0; x < width; ++x) {
                WallType type;
                switch (line[x]) {
                case '1': type = IronWall; break;
                case '2': type = WoodenWall; break;
                case '3': type = BrickWallHigh; break;
                case '4': type = BrickWallHalf; break;
                case '5': type = BrickWallLow; break;
                case '6': type = Tree; break;
                case '7': type = Ice; break;
                case '8': type = Water; break;
                case 'p': type = Player_Base; break;
                case 'e': type = Enemy_Base; break;
                default: type = Empty; break;
                }
                grid[y][x] = Wall(Position(x, y), type);
            }
        }
        in.close();
    }
    catch (const exception& e) {
        throw runtime_error("Ошибка загрузки карты: " + string(e.what()));
    }
}

int Map::getHeight() { return height; }

int Map::getWidth() { return width; }

Wall& Map::getCell(int x, int y) {
    if (x > width || x < 0 || y > height || y < 0) {
        throw out_of_range("Координаты за пределами карты.");
    }
    else return grid[x][y];
}

void Map::loadTextures() {
    // Загрузка текстур в карту
    Image image;
    image.loadFromFile("textures/walls_sprite.png");
    textures[IronWall].loadFromImage(image, IntRect(0,0,54,54));
    textures[WoodenWall].loadFromImage(image, IntRect(216, 0, 54, 54));
    textures[BrickWallHigh].loadFromImage(image, IntRect(54, 0, 54, 54));
    textures[BrickWallHalf].loadFromImage(image, IntRect(108, 0, 54, 54));
    textures[BrickWallLow].loadFromImage(image, IntRect(162, 0, 54, 54));
    textures[Ice].loadFromImage(image, IntRect(324, 0, 54, 54));
    textures[Water].loadFromImage(image, IntRect(378, 0, 54, 54));
    textures[Empty].loadFromImage(image, IntRect(432, 0, 54, 54));

    image.createMaskFromColor(Color::White);
    textures[Tree].loadFromImage(image, IntRect(270, 0, 54, 54));

    Image bases;
    bases.loadFromFile("textures/bases_sprite.png");
    bases.createMaskFromColor(Color::White);
    textures[Player_Base].loadFromImage(bases, IntRect(0, 0, 54, 54));
    textures[Enemy_Base].loadFromImage(bases, IntRect(54, 0, 54, 54));
}

void Map::draw(sf::RenderWindow& window) {
    const float cellSize = 54.0f;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Wall cell = grid[y][x];
            WallType type = cell.getType();

            // Заполняем простраство под некоторыми объектами
            if (type == Tree || type == Player_Base || type == Enemy_Base) {
                Sprite sprite;
                sprite.setTexture(textures[Empty]);
                sprite.setPosition(x * cellSize, y * cellSize);
                window.draw(sprite);
            }

            Sprite sprite;
            sprite.setTexture(textures[type]);
            sprite.setPosition(x * cellSize, y * cellSize);
            window.draw(sprite);
        }
    }
}