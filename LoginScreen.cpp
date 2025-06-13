#include "LoginScreen.h"
#include "ManagerProfile.h"
#include "UserUnitManager.h"
#include <SFML/Graphics.hpp>

using namespace std;

void loginScreen(sf::RenderWindow& window) {

	//Background Login

	sf::Texture textureBackgroundLogin;
	textureBackgroundLogin.loadFromFile("assets/images/Wallpaper Clash Royale PC.jpeg");

	sf::Sprite spriteBackgroundLogin;
	spriteBackgroundLogin.setTexture(textureBackgroundLogin);
	spriteBackgroundLogin.setPosition(0.f, 0.f);
	spriteBackgroundLogin.setScale(0.2f, 0.2f);

	sf::Vector2u textureSize = textureBackgroundLogin.getSize();
	sf::Vector2u windowSize = window.getSize();

	float scaletX = static_cast<float>(windowSize.x) / textureSize.x;
	float scaletY = static_cast<float>(windowSize.y) / textureSize.y;
	spriteBackgroundLogin.setScale(scaletX, scaletY);

	//window Login
	sf::Texture textureLogin;
	textureLogin.loadFromFile("assets/images/login.png");

	sf::Sprite spriteLogin;
	spriteLogin.setTexture(textureLogin);
	spriteLogin.setScale(0.5f, 0.5f);


	sf::Vector2u spriteSizeLogin = textureLogin.getSize();
	spriteLogin.setOrigin(spriteSizeLogin.x / 2.f, spriteSizeLogin.y / 2.f);
	spriteLogin.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);

	sf::Font font;
	font.loadFromFile("assets/fonts/CalSans-Regular.ttf");

	sf::Text nickname("Enter your nickname:", font, 30);
	nickname.setPosition(800.f, 400.f);
	nickname.setFillColor(sf::Color(200, 200, 200));

	sf::Text inputText("", font, 30);
	inputText.setPosition(800.f, 450.f);
	inputText.setFillColor(sf::Color::Yellow);

	string input;
	bool  isDone = false;

	sf::Clock cursorClock;
	bool showCursor = true;

	while (window.isOpen() && !isDone) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode == '\b') {
					if (!input.empty()) {
						input.pop_back();
					}
				}
				else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
					if (!input.empty()) {
						profileManager::setActiveUser(input);
						profileManager::savedUserData(input, "nickname", input);

						if (profileManager::loadUserData(input, "coins").empty()) {
							profileManager::savedUserData(input, "coins", "0");
						}

						if (profileManager::loadUserData(input, "trophies").empty()) {
							profileManager::savedUserData(input, "trophies", "0");
						}

						UserUnitManager::createDefaultCardsFileIfNotExists(input);
						isDone = true;
					}
				}
				else if (event.text.unicode < 128 && input.size() < 16) {
					input += static_cast<char>(event.text.unicode);
				}
			}
		}

		if (cursorClock.getElapsedTime().asMilliseconds() > 500) {
			showCursor = !showCursor;
			cursorClock.restart();
		}
		std::string display = input + (showCursor ? "|" : "");
		
		inputText.setString(display);
		window.clear(sf::Color(30, 30, 30));
		window.draw(spriteBackgroundLogin);
		window.draw(spriteLogin);
		window.draw(nickname);
		window.draw(inputText);
		window.display();
	}
}