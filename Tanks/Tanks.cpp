#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Main_menu.h"
#include "Player_Tank.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace sf;



static bool checkCollisionTank(Tank& tank, Map& map) {
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
				return true;
			}
		}
	}
	return false;
}

static bool checkCollisionBullet(Bullet& bullet, Map& map) {
	if (!bullet.getIsActive()) return false;

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
				bullet.setExpPath();
				bullet.setExpTextures();
				bullet.setExpSprite();
				bullet.startExplosion();
				
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
				return true;
			}
		}
	}
	return false;
}

static bool checkCollision_BulletsWithEnemies(Bullet& player_bullet, vector<Enemy>& enemies){
	// Размер клетки карты
	const int cellSize = 54;
	const int bulletSize = 18;

	int bulletTopLeftX = player_bullet.getPosition().getX() / cellSize;
	int bulletTopLeftY = player_bullet.getPosition().getY() / cellSize;
	int bulletBottomRightX = (player_bullet.getPosition().getX() + bulletSize - 1) / cellSize;
	int bulletBottomRightY = (player_bullet.getPosition().getY() + bulletSize - 1) / cellSize;

	FloatRect bullet_obj(player_bullet.getPosition().getX(), player_bullet.getPosition().getY(), bulletSize, bulletSize);

	int count = 0;
	for (auto enemy = enemies.begin(); enemy != enemies.end();++enemy) {
		int tankTopLeftX = enemy->getPosition().getX() / cellSize;
		int tankTopLeftY = enemy->getPosition().getY() / cellSize;
		int tankBottomRightX = (enemy->getPosition().getX() + cellSize - 1) / cellSize;
		int tankBottomRightY = (enemy->getPosition().getY() + cellSize - 1) / cellSize;

		FloatRect tank_obj(enemy->getPosition().getX(), enemy->getPosition().getY(), 49, 49);

		for (int y = tankTopLeftY; y <= tankBottomRightY; ++y) {
			for (int x = tankTopLeftX; x <= tankBottomRightX; ++x) {
				if (bullet_obj.intersects(tank_obj)) {
					//enemies.erase(enemies.begin() + count);
					

					enemies.pop_back();
					player_bullet.setIsActive(false);
					player_bullet.setExpPath();
					player_bullet.setExpTextures();
					player_bullet.setExpSprite();
					player_bullet.startExplosion();
					return true;
				}
			}
		}
		count++;
	}
	return false;
}



int main()
{

	RenderWindow window(VideoMode(1920, 1080), L"Tank1990", Style::Default);
	menu(window);
	// Загрузка карты
	Map map = Map();
	map.loadFromFile("maps/map1.txt");
	
	window.setVerticalSyncEnabled(true);
	// Инициализация игрока
	Player_Tank ptank(Position(54,54),UP,3,0,1);
	// Инициализация противников
	vector<Enemy> enemies;
	vector<Enemy>::iterator it;

	enemies.push_back(Enemy(Position(108, 108), UP, 3, 0, 1));
	enemies.push_back(Enemy(Position(972, 108), UP, 3, 0, 1));
	Image img; img.loadFromFile("textures/enemy_tank_sprite.png");
	img.createMaskFromColor(Color::Black);
	Texture text; text.loadFromImage(img);
	enemies[0].getSprite().setTexture(text);
	enemies[0].getSprite().setTexture(text);
	for (auto& enemy : enemies) {
		enemy.getBullets().resize(AMOUNT_OF_BULLETS);
		for (int i = 0; i < AMOUNT_OF_BULLETS; i++) {
			enemy.getBullets()[i] = Bullet(Position(0, 0), UP, 0.2);
			//Для отрисовки снаряда
			enemy.getBullets()[i].setPath();
			enemy.getBullets()[i].setTextures();
			enemy.getBullets()[i].setSprite();
			enemy.getBullets()[i].getSprite().setPosition(0, 0);
		}
	}

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

		for (it = enemies.begin(); it != enemies.end(); it++) {
			if (checkCollisionTank((*it), map)) {
				//(*it)->setDirection(static_cast<Direction>(rand() % 4));
				(it)->startRandomMovement();
			}
			(it)->enemy_control(time);
			for (auto& bullet : (it)->getBullets()) {
				checkCollisionBullet(bullet, map);
				bullet.updateExplosion(time);
				bullet.renderExplosion(window);
				if (bullet.getIsActive() && !bullet.checkBoarderCollision(bullet.getPosition().getX(), bullet.getPosition().getY(), bullet.getDirection(), bullet.getSpeed(), time)) {
					bullet.move(time);
					window.draw(bullet.getSprite());
				}
			}

			window.draw((it)->getSprite());
		}
		
		// Проверка коллизий
		checkCollisionTank(ptank, map);
		
		for (auto& bullet : ptank.getBullets()) {
			checkCollisionBullet(bullet, map);
			bullet.updateExplosion(time);
			bullet.renderExplosion(window);
		}

		// Отрисовка снарядов
		for (auto& bullet : ptank.getBullets()) {
			if (bullet.getIsActive() && !bullet.checkBoarderCollision(bullet.getPosition().getX(), bullet.getPosition().getY(), bullet.getDirection(), bullet.getSpeed(), time)) {
				bullet.move(time);
				window.draw(bullet.getSprite());
				checkCollision_BulletsWithEnemies(bullet, enemies);
			}
		
		}

		// Отрисовываем игрока
		window.draw(ptank.getSprite());

		window.display();
	}
	return 0;
}