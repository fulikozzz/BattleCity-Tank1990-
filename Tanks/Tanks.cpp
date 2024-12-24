#include <SFML/Graphics.hpp>
#include "Map.h"

#include "Player_Tank.h"
#include "Bullet.h"

using namespace sf;

static void checkCollisionTank(Tank& tank, Map& map) {
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

void checkCollisionBullet(Bullet& bullet, Map& map) {
	if (!bullet.getIsActive()) return;

	// Размер клетки карты
	const int cellSize = 54;
	const int bulletSize = 18;

	int bulletTopLeftX = bullet.getPosition().getX() / cellSize;
	int bulletTopLeftY = bullet.getPosition().getY() / cellSize;
	int bulletBottomRightX = (bullet.getPosition().getX() + bulletSize - 1) / cellSize;
	int bulletBottomRightY = (bullet.getPosition().getY() + bulletSize - 1) / cellSize;

	FloatRect bullet_obj(bullet.getPosition().getX(), bullet.getPosition().getY(), bulletSize, bulletSize);

	for (int y = bulletTopLeftY; y <= bulletBottomRightY; ++y) {
		for (int x = bulletTopLeftX; x <= bulletBottomRightX; ++x) {
			Wall wall = map.getCell(y, x);
			if (wall.getType() == Empty || wall.getType() == Tree || wall.getType() == Ice || wall.getType() == Water) {
				continue;
			}

			FloatRect walls(x * cellSize, y * cellSize, cellSize, cellSize);

			if (walls.intersects(bullet_obj)) {

				bullet.setIsActive(false);
				
				WallType type = wall.getType();
				switch (type)
				{
				case WoodenWall:
					map.getCell(y, x).setType(Empty);
					break;
				case BrickWallHigh:
					map.getCell(y, x).setType(BrickWallHalf);
					break;
				case BrickWallHalf:
					map.getCell(y, x).setType(BrickWallLow);
					break;
				case BrickWallLow:
					map.getCell(y, x).setType(Empty);
					break;
				default:
					break;
				}
				return;
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

		//Отрисовка карты
		map.draw(window);
		
		// Включаем контроллер
		ptank.control(time);
		
		// Проверка коллизий
		checkCollisionTank(ptank, map);
		for (auto& bullet : ptank.getBullets()) {
			checkCollisionBullet(bullet, map);
		}

		// Отрисовка снарядов
		for (auto& bullet : ptank.getBullets()) {
			if (bullet.getIsActive() && !bullet.checkBoarderCollision(bullet.getPosition().getX(), bullet.getPosition().getY(), bullet.getDirection(), bullet.getSpeed(), time)) {
				bullet.move(time);
				window.draw(bullet.getSprite());
			}
		}
		// Отрисовываем игрока
		window.draw(ptank.getSprite());

		window.display();
	}
	return 0;
}