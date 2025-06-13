#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class SettingsWindow {
private:
    sf::Sprite windowSprite;
    sf::Texture backgroundTexture;
    sf::Font font;
    sf::Text logoutText;
    sf::Vector2u windowSize;
    
    sf::Texture btnClouse;
    sf::Sprite spriteBtnClouse;

    sf::RectangleShape volumeBar;
    sf::CircleShape btnVolume;
    float volume;
    bool isDragging = false;

public:
    SettingsWindow(const sf::Vector2u& winSize);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event, bool& showSettings, bool& logoutTriggered);
    void draw(sf::RenderWindow& window);
};
