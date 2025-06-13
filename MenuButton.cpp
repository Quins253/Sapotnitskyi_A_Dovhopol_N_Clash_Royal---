#include "MenuButton.h"

MenuButton::MenuButton(sf::Texture* texture, sf::Vector2f position, sf::Vector2f size)
	: texture(texture), position(position), size(size) {
	sprite.setTexture(*texture);
	sprite.setPosition(position);
	sprite.setScale(size.x / texture->getSize().x, size.y / texture->getSize().y);
}


void MenuButton::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

bool MenuButton::isMouseOver(sf::RenderWindow& window) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	return sprite.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
}

void MenuButton::setHoverEffect(bool isHover) {
	if (isHover) {
		sprite.setColor(sf::Color(200, 200, 255));
	}
	else {
		sprite.setColor(sf::Color::White);
	}
}

sf::FloatRect MenuButton::getBounds() const {
    return sprite.getGlobalBounds();
}