#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class MenuButton {
private:
	sf::Texture* texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	sf::Vector2f size;

public:
	MenuButton(sf::Texture* texture, sf::Vector2f position, sf::Vector2f size);

	void draw(sf::RenderWindow& window);
	bool isMouseOver(sf::RenderWindow& window);
	void setHoverEffect(bool isHover);
	sf::FloatRect getBounds() const;
};