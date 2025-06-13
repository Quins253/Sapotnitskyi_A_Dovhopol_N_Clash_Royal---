#include "UserProfile.h"
#include <iostream>

using namespace std;

UserProfile::UserProfile(sf::Texture* textureBanner, const string& name)
    : texture(textureBanner)
{
    if (texture) {
        sprite.setTexture(*texture);
        sprite.setScale(0.8f, 0.8f);
        sprite.setPosition(0.f, 35.f);
    }
    else {
        cout << "Error: texture pointer is null\n";
    }

    font.loadFromFile("assets/fonts/CalSans-Regular.ttf");

    playerName.setFont(font);
    playerName.setString(name);
    playerName.setCharacterSize(24);
    playerName.setFillColor(sf::Color::White);
    playerName.setPosition(40.f, 250.f);
}

void UserProfile::setTexture(sf::Texture* newTexture) {
    texture = newTexture;
    if (texture) {
        sprite.setTexture(*texture);
    }
}

void UserProfile::userDraw(sf::RenderWindow& window) {
    if (texture) {
        window.draw(sprite);
    }
    window.draw(playerName);
}
