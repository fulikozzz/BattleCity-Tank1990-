#include <SFML/Graphics.hpp>
#include "Map.h"

#include "Player_Tank.h"

using namespace sf;

void checkCollisionTank(Tank& tank, Map& map) {
	/*for (int y = (tank.getPosition().getY() + 5) / 54; y < (tank.getPosition().getY() + 49) / 54; y++) {
		for (int x = (tank.getPosition().getX() + 5) / 54; x < (tank.getPosition().getX() + 49) / 54; x++) {
			Wall rect = map.getCell(y, x);
			if (rect.getType() == Empty || rect.getType() == Tree || rect.getType() == Ice) {
				continue;
			}
			if (tank.getDirection() == UP) {
				tank.setPosition(Position(tank.getPosition().getX(), y * 54 + 49));
			}
			if (tank.getDirection() == LEFT) {
				tank.setPosition(Position(x * 54 + 49, tank.getPosition().getY()));
			}
			if (tank.getDirection() == DOWN) {
				tank.setPosition(Position(tank.getPosition().getX(), y * 54 - 49));
			}
			if (tank.getDirection() == RIGHT) {
				tank.setPosition(Position(x * 54 - 49, tank.getPosition().getY()));
			}
		}
	}*/
	// Размер клетки карты
	const int cellSize = 54;

	int tankTopLeftX = tank.getPosition().getX() / cellSize;
	int tankTopLeftY = tank.getPosition().getY() / cellSize;
	int tankBottomRightX = (tank.getPosition().getX() + cellSize - 1) / cellSize;
	int tankBottomRightY = (tank.getPosition().getY() + cellSize - 1) / cellSize;

	FloatRect tank_obj(tank.getPosition().getX(), tank.getPosition().getY(), 49, 49);
	
	for (int y = tankTopLeftY; y <= tankBottomRightY; ++y) {
		for (int x = tankTopLeftX; x <= tankBottomRightX; ++x) {
			Wall wall = map.getCell(y, x);
			if (wall.getType() == Empty || wall.getType() == Tree || wall.getType() == Ice) {
				continue; 
			}
			
			FloatRect walls(x * cellSize, y * cellSize, cellSize, cellSize);

			if (walls.intersects(tank_obj)) {
				if (tank.getDirection() == UP) {
					tank.setPosition(Position(tank.getPosition().getX(), walls.top + walls.height));
				}
				else if (tank.getDirection() == DOWN) {
					tank.setPosition(Position(tank.getPosition().getX(), walls.top - tank_obj.height));
				}
				else if (tank.getDirection() == LEFT) {
					tank.setPosition(Position(walls.left + walls.width, tank.getPosition().getY()));
				}
				else if (tank.getDirection() == RIGHT) {
					tank.setPosition(Position(walls.left - tank_obj.width, tank.getPosition().getY()));
				}
			}
		}
	}
}


int main()
{

	RenderWindow window(VideoMode(1920, 1080), L"Tank1990", Style::Default);
	// Загрузка карты
	Map map = Map();
	map.loadFromFile("maps/map1.txt");
	
	window.setVerticalSyncEnabled(true);
	// Инициализация игрока
	Player_Tank ptank(Position(54,54),UP,3,0,1);

	// Часы для привязки ко времени
	Clock clock;

	while (window.isOpen())
	{
		// Привязка ко времени
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear(Color::Blue);
		map.draw(window);
		// Включаем контроллер
		ptank.control(time);
		checkCollisionTank(ptank, map);
		// Отрисовываем игрока
		window.draw(ptank.getSprite());
		window.display();
	}
	return 0;
}