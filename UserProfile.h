#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UserProfile {
private:
    sf::Texture* texture;
    sf::Sprite sprite;
    sf::Font font;
    sf::Text playerName;

public:
    UserProfile(sf::Texture* textureBanner, const std::string& name);

    void setTexture(sf::Texture* newTexture);
    void userDraw(sf::RenderWindow& window);
};
