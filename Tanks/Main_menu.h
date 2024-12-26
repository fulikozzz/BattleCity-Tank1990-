#include <SFML/Graphics.hpp>
#include <chrono>
using namespace sf;


chrono::steady_clock::time_point lastClick;  //время последнего выстрела
const chrono::milliseconds clickCooldown = std::chrono::milliseconds(100);
void menu(RenderWindow& window) {
	window.setVerticalSyncEnabled(true);
	Texture menuTexture1, menuBackground;
	menuTexture1.loadFromFile("textures/menu_tank_sprite.png");
	menuBackground.loadFromFile("textures/main_menu_sprite.png");
	Sprite menu1(menuTexture1), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(620, 430);
	menuBg.setPosition(0, 0);
	lastClick = std::chrono::steady_clock::now();



	//////////////////////////////МЕНЮ///////////////////
	while (isMenu) {
		auto now = std::chrono::steady_clock::now();
		if (now - lastClick < clickCooldown) {
			continue;
		}
	
		
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				if (menu1.getPosition().y - 110 >= 430) {
					menu1.setPosition(620, menu1.getPosition().y - 110);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				if (menu1.getPosition().y + 110 <= 760) {
					menu1.setPosition(620, menu1.getPosition().y + 110);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				if (menu1.getPosition().y == 430) {
					isMenu = false;
				}
				if (menu1.getPosition().y == 760) {
					window.close();
					isMenu = false;
				}

			}

		window.draw(menuBg);
		window.draw(menu1);
		window.display();
		lastClick = now;

	}
}

	

