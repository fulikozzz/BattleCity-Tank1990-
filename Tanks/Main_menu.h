#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>

using namespace sf;


chrono::steady_clock::time_point lastClick;  //время последнего переключения
const chrono::milliseconds clickCooldown = std::chrono::milliseconds(100);
void menu(RenderWindow& window) {
	SoundBuffer buffer;
	Sound sound;
	buffer.loadFromFile("audio/main_menu_sound.wav");
	sound.setBuffer(buffer);
	sound.setVolume(8);
	sound.play();
	
	window.setVerticalSyncEnabled(true);
	Texture menuTexture1, menuBackground;
	menuTexture1.loadFromFile("textures/menu_tank_sprite.png");
	menuBackground.loadFromFile("textures/main_menu.png");
	Sprite menu1(menuTexture1), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(620, 465);
	menuBg.setPosition(0, 0);
	lastClick = std::chrono::steady_clock::now();



	//////////////////////////////МЕНЮ///////////////////
	while (isMenu) {
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized)
				window.setSize(Vector2u(event.size.width, event.size.height));

		}
		auto now = std::chrono::steady_clock::now();
		if (now - lastClick < clickCooldown) {
			continue;
		}
	

			if (Keyboard::isKeyPressed(Keyboard::W)) {
				if (menu1.getPosition().y - 220 >= 465) {
					menu1.setPosition(620, menu1.getPosition().y - 220);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				if (menu1.getPosition().y + 220 <= 760) {
					menu1.setPosition(620, menu1.getPosition().y + 220);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				if (menu1.getPosition().y == 465) {
					isMenu = false;
				}
				if (menu1.getPosition().y == 685) {
					window.close();
					isMenu = false;
				}

			}
		
		window.clear();
		window.draw(menuBg);
		window.draw(menu1);
		window.display();
		lastClick = now;

	}
}

	

