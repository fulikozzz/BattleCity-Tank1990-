#include <SFML/Graphics.hpp>
#include "Map.h"

#include "Player_Tank.h"
using namespace sf;

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
		// Отрисовываем игрока
		window.draw(ptank.getSprite());
		window.display();
	}
	return 0;
}