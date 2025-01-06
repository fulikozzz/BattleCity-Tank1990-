#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Main_menu.h"
#include "Player_Tank.h"
#include "Enemy.h"

#include <algorithm>
#include <string>
#include <string.h>
#include <Windows.h>

using namespace sf;

static bool checkCollisionTank(Tank& tank, Map& map) {
	const int cellSize = 54;

	int tankTopLeftX = tank.getPosition().getX() / cellSize;
	int tankTopLeftY = tank.getPosition().getY() / cellSize;
	int tankBottomRightX = (tank.getPosition().getX() + cellSize - 1) / cellSize;
	int tankBottomRightY = (tank.getPosition().getY() + cellSize - 1) / cellSize;

	FloatRect tank_obj(tank.getPosition().getX(), tank.getPosition().getY(), 49, 49);
	
	for (int y = tankTopLeftY; y <= tankBottomRightY; ++y) {
		for (int x = tankTopLeftX; x <= tankBottomRightX; ++x) {
			Wall wall = map.getCell(y, x);
			FloatRect walls(x * cellSize, y * cellSize, cellSize, cellSize);
			if (wall.getType() == Empty || wall.getType() == Tree) {
				tank.setSpeed(0.1);
				continue;
			}
			else if (wall.getType() == Ice && walls.intersects(tank_obj)) {
				tank.setSpeed(0.03);
				continue;
			}
			else if (walls.intersects(tank_obj)) {
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
				case Player_Base:
					map.getPlayerBase().setIsDestroyed(true);
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
					enemies.erase(enemies.begin() + count);

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

static bool checkCollision_BulletsWithPlayer(Bullet& enemy_bullet, Player_Tank& player) {
	const int cellSize = 54;
	const int bulletSize = 18;

	int bulletTopLeftX = enemy_bullet.getPosition().getX() / cellSize;
	int bulletTopLeftY = enemy_bullet.getPosition().getY() / cellSize;
	int bulletBottomRightX = (enemy_bullet.getPosition().getX() + bulletSize - 1) / cellSize;
	int bulletBottomRightY = (enemy_bullet.getPosition().getY() + bulletSize - 1) / cellSize;

	FloatRect bullet_obj(enemy_bullet.getPosition().getX(), enemy_bullet.getPosition().getY(), bulletSize, bulletSize);
	
		int tankTopLeftX = player.getPosition().getX() / cellSize;
		int tankTopLeftY = player.getPosition().getY() / cellSize;
		int tankBottomRightX = (player.getPosition().getX() + cellSize - 1) / cellSize;
		int tankBottomRightY = (player.getPosition().getY() + cellSize - 1) / cellSize;

		FloatRect tank_obj(player.getPosition().getX(), player.getPosition().getY(), 49, 49);

		for (int y = tankTopLeftY; y <= tankBottomRightY; ++y) {
			for (int x = tankTopLeftX; x <= tankBottomRightX; ++x) {
				if (bullet_obj.intersects(tank_obj)) {
					enemy_bullet.setIsActive(false);
					enemy_bullet.setExpPath();
					enemy_bullet.setExpTextures();
					enemy_bullet.setExpSprite();
					enemy_bullet.startExplosion();
					if (player.getLives() > 0) player.setLives(player.getLives() - 1);
					return true;
				}
			}
		}
	
	return false;
}

static bool checkCollisionWithEnemies(Tank& playerTank, vector<Enemy>& enemies) {
	const int cellSize = 54;

	int tankTopLeftX = playerTank.getPosition().getX() / cellSize;
	int tankTopLeftY = playerTank.getPosition().getY() / cellSize;
	int tankBottomRightX = (playerTank.getPosition().getX() + cellSize - 1) / cellSize;
	int tankBottomRightY = (playerTank.getPosition().getY() + cellSize - 1) / cellSize;

	FloatRect tank_obj(playerTank.getPosition().getX(), playerTank.getPosition().getY(), 54, 54);

	for (int y = tankTopLeftY; y <= tankBottomRightY; ++y) {
		for (int x = tankTopLeftX; x <= tankBottomRightX; ++x) {

			for (auto& enemy : enemies) {

				int tankTopLeftX = enemy.getPosition().getX() / cellSize;
				int tankTopLeftY = enemy.getPosition().getY() / cellSize;
				int tankBottomRightX = (enemy.getPosition().getX() + cellSize - 1) / cellSize;
				int tankBottomRightY = (enemy.getPosition().getY() + cellSize - 1) / cellSize;

				FloatRect enemy_obj(enemy.getPosition().getX(), enemy.getPosition().getY(), 54, 54);

				if (enemy_obj.intersects(tank_obj)) {
						if (playerTank.getDirection() == UP && enemy.getDirection() == DOWN) {
							playerTank.setPosition(Position(playerTank.getPosition().getX(), tank_obj.top));
							enemy.setPosition(Position(enemy.getPosition().getX(), tank_obj.top - tank_obj.height));
						}
						else if (playerTank.getDirection() == LEFT && enemy.getDirection() == RIGHT) {
							playerTank.setPosition(Position(tank_obj.left, playerTank.getPosition().getY()));
							enemy.setPosition(Position(tank_obj.left - enemy_obj.width, enemy.getPosition().getY()));
						}
						else if (playerTank.getDirection() == RIGHT && enemy.getDirection() == LEFT) {
							playerTank.setPosition(Position(tank_obj.left, playerTank.getPosition().getY()));
							enemy.setPosition(Position(tank_obj.left + enemy_obj.width, enemy.getPosition().getY()));
						}
						else if (playerTank.getDirection() == DOWN && enemy.getDirection() == UP) {
							playerTank.setPosition(Position(playerTank.getPosition().getX(), tank_obj.top));
							enemy.setPosition(Position(enemy.getPosition().getX(), tank_obj.top + tank_obj.height));
						}

					playerTank.getSprite().setPosition(playerTank.getPosition().getX(), playerTank.getPosition().getY());
					enemy.getSprite().setPosition(enemy.getPosition().getX(), enemy.getPosition().getY());
					return true;
				}
			}
		}
	}
	return false;
}

void drawRightPanel(sf::RenderWindow& window, int score, int enemiesKilled, int enemiesRemaining, int playerLives, int playerArmor, bool baseDestroyed, int current_level) {
	// Размер окна
	Vector2u windowSize = window.getSize();

	// Размеры панели
	const int panelWidth = 840;
	const int panelHeight = 1080;

	// Прямоугольник для фона правой панели
	sf::RectangleShape panelBackground(sf::Vector2f(panelWidth, panelHeight));
	panelBackground.setFillColor(sf::Color(50, 50, 50)); // Тёмно-серый цвет
	panelBackground.setPosition(windowSize.x - panelWidth, 0);

	// Шрифт
	sf::Font font;
	if (!font.loadFromFile("fonts/arial.ttf")) {
		throw std::runtime_error("Failed to load font");
	}

	// Увеличение шрифта для элементов
	const int largeFontSize = 72;
	const int mediumFontSize = 42;
	const int smallFontSize = 36;

	// Текст для уровня
	sf::Text levelText;
	levelText.setFont(font);
	levelText.setString("LEVEL " + std::to_string(current_level));
	levelText.setCharacterSize(largeFontSize);
	levelText.setFillColor(sf::Color::White);
	levelText.setPosition(windowSize.x - panelWidth + 240, 5);

	// Текст для счёта
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setString("SCORE: " + std::to_string(score));
	scoreText.setCharacterSize(largeFontSize);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(windowSize.x - panelWidth + 240, 580);

	// Текст для характеристики игрока
	sf::Text statisticText;
	statisticText.setFont(font);
	statisticText.setString("Your conditions");
	statisticText.setCharacterSize(mediumFontSize);
	statisticText.setFillColor(sf::Color::White);
	statisticText.setPosition(windowSize.x - panelWidth + 270, 710);

	// Текст для жизней игрока
	sf::Text playerLivesText;
	playerLivesText.setFont(font);
	playerLivesText.setString("Lives: " + std::to_string(playerLives));
	playerLivesText.setCharacterSize(mediumFontSize);
	playerLivesText.setFillColor(sf::Color::White);
	playerLivesText.setPosition(windowSize.x - panelWidth + 100, 830);

	// Текст для уровня брони игрока
	sf::Text playerArmorText;
	playerArmorText.setFont(font);
	playerArmorText.setString("Armor: " + std::to_string(playerArmor));
	playerArmorText.setCharacterSize(mediumFontSize);
	playerArmorText.setFillColor(sf::Color::White);
	playerArmorText.setPosition(windowSize.x - panelWidth + 580, 830);

	// Текст для состояния базы
	sf::Text baseStatusText;
	baseStatusText.setFont(font);
	baseStatusText.setString("Base: " + std::string(baseDestroyed ? "Destroyed" : "Active"));
	baseStatusText.setCharacterSize(mediumFontSize);
	baseStatusText.setFillColor(baseDestroyed ? sf::Color::Red : sf::Color::Green);
	baseStatusText.setPosition(windowSize.x - panelWidth + 300, 950);

	// Текст для убитых врагов
	sf::Text killedText;
	killedText.setFont(font);
	killedText.setString("Enemies destroyed: " + std::to_string(enemiesKilled));
	killedText.setCharacterSize(mediumFontSize);
	killedText.setFillColor(sf::Color::White);
	killedText.setPosition(windowSize.x - panelWidth + 80, 105);

	// Текст для оставшихся врагов
	sf::Text remainingText;
	remainingText.setFont(font);
	remainingText.setString("Enemies remaining: " + std::to_string(enemiesRemaining));
	remainingText.setCharacterSize(mediumFontSize);
	remainingText.setFillColor(sf::Color::White);
	remainingText.setPosition(windowSize.x - panelWidth + 80, 155);

	// Отрисовка правой панели
	window.draw(panelBackground);
	
	// Отрисовка противников на панели
	sf::Image img;
	sf::Texture texture;
	sf::Sprite enemyTank;
	img.loadFromFile("textures/enemy_tank_panel.png");
	img.createMaskFromColor(Color::Black);
	texture.loadFromImage(img);
	enemyTank.setTexture(texture);
	
	// Параметры отрисовки
	const int tankSize = 81;  // Размер танка
	const int tankMargin = 10; // Отступ между танками
	const int tanksPerColumn = 9; // Количество танков в одном ряду
	const int tankStartY = 255;  // Начальная позиция для отрисовки танков

	// Вычисление количества рядов
	int rows = (enemiesRemaining + tanksPerColumn - 1) / tanksPerColumn;
	int remainingTanks = enemiesRemaining;

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < tanksPerColumn && remainingTanks > 0; ++col) {
			// Вычисление позиции танка в зависимости от строки и колонки
			float xPos = windowSize.x - panelWidth + 10 + col * (tankSize + tankMargin);
			float yPos = tankStartY + row * (tankSize + tankMargin);

			// Устанавливаем позицию танка
			enemyTank.setPosition(xPos, yPos);

			// Отрисовываем танк
			window.draw(enemyTank);

			remainingTanks--;
		}
	}
	window.draw(levelText);
	window.draw(statisticText);
	window.draw(scoreText);
	window.draw(killedText);
	window.draw(remainingText);
	window.draw(playerLivesText);
	window.draw(playerArmorText);
	window.draw(baseStatusText);
}

bool check_Winner(Player_Tank& player, vector<Enemy>& enemies, Map& map, int& level, RenderWindow& window, int& enemiesToSpawn, Clock& enemySpawnClock) {
	Font font;
	if (!font.loadFromFile("fonts/arial.ttf")) {
		throw runtime_error("Failed to load font");
	}

	if (enemies.empty() && enemiesToSpawn <= 0) {
		player.getEngineSound().stop();
		player.getIdleSound().stop();
		if (level < 5) {
			++level;
			map.loadFromFile("maps/map" + to_string(level) + ".txt");
			player.setPosition(Position((map.getPlayerBase().getPositon().getX() - 2) * 54, (map.getPlayerBase().getPositon().getY()) * 54));
			player.getSprite().setPosition((map.getPlayerBase().getPositon().getX() - 2) * 54, (map.getPlayerBase().getPositon().getY()) * 54);
			player.setDirection(UP); 
			player.setLives(player.getLives() + 1);
			window.draw(player.getSprite());
			window.display();
			enemies.clear();
			enemiesToSpawn = 5 * level;
			enemySpawnClock.restart();
			for (auto& bullet : player.getBullets()) {
				bullet.setIsActive(false);
				bullet.setIsExploding(false);
			}
		}
		else {
			Text winText;
			winText.setFont(font);
			winText.setString("   \tCONGRATULATIONS!\
				\nYou have completed the game!\n\n\n\n\n\n\n\tPress any key to continue...");
			winText.setCharacterSize(48);
			winText.setFillColor(sf::Color::Green);
			winText.setPosition(window.getSize().x / 2 - winText.getGlobalBounds().width / 2,
				window.getSize().y / 2 - winText.getGlobalBounds().height / 4);

			// Отображаем окно победы
			window.clear(Color(50, 50, 50));
			window.draw(winText);
			window.display();

			// Ожидаем нажатие клавиши
			while (window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::KeyPressed || event.type == sf::Event::Closed) {
						// Возвращаемся в меню
						player.setPosition(Position((map.getPlayerBase().getPositon().getX() - 2) * 54, (map.getPlayerBase().getPositon().getY()) * 54));
						player.getSprite().setPosition((map.getPlayerBase().getPositon().getX() - 2) * 54, (map.getPlayerBase().getPositon().getY()) * 54);
						level = 1;
						player.setDirection(UP);
						player.setLives(3);
						player.setScore(0);
						enemies.clear();
						enemiesToSpawn = 5 * level;
						enemySpawnClock.restart();
						map.loadFromFile("maps/map" + to_string(level) + ".txt");
						menu(window);
						for (auto& bullet : player.getBullets()) {
							bullet.setIsActive(false);
							bullet.setIsExploding(false);
						}
						return true;
					}
				}
			}
		}
		return true;
	}
	else if (player.getLives() == 0 || map.getPlayerBase().getIsDestroyed()) {
		player.setPosition(Position((map.getPlayerBase().getPositon().getX() - 2) * 54, (map.getPlayerBase().getPositon().getY()) * 54));
		player.getSprite().setPosition((map.getPlayerBase().getPositon().getX() - 2) * 54, (map.getPlayerBase().getPositon().getY()) * 54);
		player.setDirection(UP);
		player.setLives(3);
		player.setScore(0);
		player.getEngineSound().stop();
		player.getIdleSound().stop();
		level = 1;
		map.loadFromFile(("maps/map" + to_string(level) + ".txt"));
		enemies.clear();
		enemiesToSpawn = 5 * level;
		enemySpawnClock.restart();

		// Отображаем экран поражения
		Text loseText;
		loseText.setFont(font);
		loseText.setString("\tGAME OVER!\
			\n\t We are sorry. \n\n\n\n\n\n\nPress any key to restart...");
		loseText.setCharacterSize(48);
		loseText.setFillColor(sf::Color::Red);
		loseText.setPosition(window.getSize().x / 2 - loseText.getGlobalBounds().width / 2,
			window.getSize().y / 2 - loseText.getGlobalBounds().height / 4);

		window.clear(Color(50, 50, 50));
		window.draw(loseText);
		window.display();

		// Ожидаем нажатие клавиши
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::Closed) {
					// Возвращаемся в меню
					menu(window);
					return false;
				}
			}
		}
		return false;
	}
	return false;
}

int main() {
	RenderWindow window(VideoMode(1920, 1080), L"Tank1990", Style::Default);
	window.setVerticalSyncEnabled(true);
	Image icon;
	// Загрузка иконки приложения
	if (!icon.loadFromFile("textures/menu_tank_sprite.png")) {
		cerr << "Failed to load icon!" << std::endl;
		return -1;
	}
	window.setIcon(60, 60, icon.getPixelsPtr());

	int current_level = 1;
	Map map;
	map.loadFromFile("maps/map" + to_string(current_level) + ".txt");
	Player_Tank ptank(Position((map.getPlayerBase().getPositon().getX() - 2) * 54, (map.getPlayerBase().getPositon().getY()) * 54), UP, 3, 0, 1);
	vector<Enemy> enemies;

	Clock clock;
	Clock enemySpawnClock;
	float time;
	const float spawnInterval = 3.0f;  // Интервал появления врагов в секундах
	int enemiesToSpawn = 5 * current_level;
	int score = 0;
	int enemiesKilled = 0;
	int enemiesRemains;

	menu(window);

	while (window.isOpen()) {
		enemiesRemains = enemiesToSpawn + enemies.size();
		//if (Keyboard::isKeyPressed(Keyboard::Tilde)) ptank.setLives(20000);
		//if (Keyboard::isKeyPressed(Keyboard::F1)) enemies.clear();
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 800;

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear(Color::Blue);
		// Отрисовка карты
		map.draw(window);
		// Спавн противников
		if (enemiesToSpawn > 0 && enemySpawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
			Position enemyBasePos = map.getEnemyBase().getPositon();
			Position spawnPos;
			if (rand() % 2 == 0)
				spawnPos = Position((enemyBasePos.getX() + 2) * 54, enemyBasePos.getY() * 54);
			else
				spawnPos = Position((enemyBasePos.getX() - 2) * 54, enemyBasePos.getY() * 54);
			Enemy enemy = Enemy(spawnPos, DOWN, 1, 0, rand() % 4);
			if (!checkCollisionTank(enemy, map)) {
				enemies.push_back(enemy);
				enemySpawnClock.restart();
				enemiesToSpawn--;
			}
		}
		// Движение игрока
		if (!checkCollisionTank(ptank, map)) {
			ptank.control(time);
		}
		// Коллизия с противниками
		//checkCollisionWithEnemies(ptank, enemies);
		// Движение противников
		for (auto it = enemies.begin(); it != enemies.end(); ++it) {
			if (checkCollisionTank((*it), map) || it->checkBoarderCollision(it->getPosition().getX(), it->getPosition().getY(), it->getDirection(), it->getSpeed(), time)) {
				it->startRandomMovement();
			}
			it->enemy_control(time, map.getPlayerBase());
			for (auto& bullet : it->getBullets()) {
				checkCollisionBullet(bullet, map);
				bullet.updateExplosion(time);
				bullet.renderExplosion(window);
				if (bullet.getIsActive() && !bullet.checkBoarderCollision(bullet.getPosition().getX(), bullet.getPosition().getY(), bullet.getDirection(), bullet.getSpeed(), time)) {
					bullet.move(time);
					checkCollision_BulletsWithPlayer(bullet, ptank);
					window.draw(bullet.getSprite());
				}
			}
			window.draw(it->getSprite());
		}
		// Анимация взрывов
		for (auto& bullet : ptank.getBullets()) {
			checkCollisionBullet(bullet, map);
			bullet.updateExplosion(time);
			bullet.renderExplosion(window);
		}
		// Стрельба игрока
		for (auto& bullet : ptank.getBullets()) {
			if (bullet.getIsActive() && !bullet.checkBoarderCollision(bullet.getPosition().getX(), bullet.getPosition().getY(), bullet.getDirection(), bullet.getSpeed(), time)) {
				bullet.move(time);
				window.draw(bullet.getSprite());
				if (checkCollision_BulletsWithEnemies(bullet, enemies)) {
					ptank.setScore(ptank.getScore() + 100);
					enemiesKilled++;
					enemiesRemains--;
				}
			}
		}
		// Проверка победы
		if(check_Winner(ptank, enemies, map, current_level, window, enemiesToSpawn, enemySpawnClock)) enemiesKilled = 0;
		// Отрисовка панели статистики
		drawRightPanel(window, ptank.getScore(), enemiesKilled, enemiesRemains, ptank.getLives(), ptank.getArmor(), map.getPlayerBase().getIsDestroyed(), current_level);
		// Отрисовка игрока
		window.draw(ptank.getSprite());
		// Отрисовка текстур деревьев
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				Wall wall = map.getCell(y, x);
				if (wall.getType() == Tree) {
					Sprite treeSprite;
					auto it = map.getMap().find(Tree);
					if (it != map.getMap().end()) {
						treeSprite.setTexture(it->second);
					}
					treeSprite.setPosition(x * 54, y * 54);
					window.draw(treeSprite);
				}
			}
		}
		
		window.display();
	}
	return 0;
}
