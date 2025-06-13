#include "Setting.h"
#include "ManagerProfile.h"
#include <iostream>

extern sf::Music musicMainMenu;

SettingsWindow::SettingsWindow(const sf::Vector2u& winSize) : windowSize(winSize) {
    //setting window
    backgroundTexture.loadFromFile("assets/images/login.png");
    windowSprite.setTexture(backgroundTexture);
    windowSprite.setScale(0.5f, 0.5f);
    windowSprite.setOrigin(backgroundTexture.getSize().x / 2.f, backgroundTexture.getSize().y / 2.f);
    windowSprite.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);

    //logout btn
    font.loadFromFile("assets/fonts/CalSans-Regular.ttf");
    logoutText.setFont(font);
    logoutText.setString("Logout");
    logoutText.setCharacterSize(32);
    logoutText.setFillColor(sf::Color::Red);
    logoutText.setPosition(windowSize.x / 2.f - 40.f, windowSize.y / 2.f + 230);
    
    //Btn close
    btnClouse.loadFromFile("assets/icons/exist_btn.jpg");
    spriteBtnClouse.setTexture(btnClouse);
    spriteBtnClouse.setScale(0.89f, 0.89f);
    spriteBtnClouse.setPosition(windowSize.x / 2.f + 215.f, windowSize.y / 2.f - 330.f);

    //Volume Bar
    volume = musicMainMenu.getVolume();

    volumeBar.setSize(sf::Vector2f(200.f, 5.f));
    volumeBar.setFillColor(sf::Color::Black);
    volumeBar.setPosition(windowSize.x / 2 - 100.f, windowSize.y / 2 + 150.f);

    btnVolume.setRadius(10.f);
    btnVolume.setFillColor(sf::Color::Red);
    btnVolume.setOrigin(10.f, 10.f);
    btnVolume.setPosition(volumeBar.getPosition().x + (volume/100.f) * 200.f, volumeBar.getPosition().y + 2.5f);
}

void SettingsWindow::handleEvent(sf::RenderWindow& window, const sf::Event& event, bool& showSettings, bool& logoutTriggered) {
    sf::Vector2f mousePositionF(static_cast<float>(sf::Mouse::getPosition(window).x),
        static_cast<float>(sf::Mouse::getPosition(window).y));

    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (logoutText.getGlobalBounds().contains(mousePositionF)) {
            profileManager::clearActiveUser();
            logoutTriggered = true;
        }

        if (spriteBtnClouse.getGlobalBounds().contains(mousePositionF)) {
            showSettings = false;
        }

        if (btnVolume.getGlobalBounds().contains(mousePositionF)) {
            isDragging = true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        isDragging = false;
    }

    if (event.type == sf::Event::MouseMoved && isDragging) {
        float x = mousePositionF.x;
        float left = volumeBar.getPosition().x;
        float right = left + volumeBar.getSize().x;

        if (x < left) {
            x = left;
        }
        if (x > right) {
            x = right;
        }

        btnVolume.setPosition(x, volumeBar.getPosition().y + volumeBar.getSize().y / 2.f);

        volume = ((x - left) / volumeBar.getSize().x) * 100.f;
        musicMainMenu.setVolume(volume);
    }
}

void SettingsWindow::draw(sf::RenderWindow& window) {
    window.draw(windowSprite);
    window.draw(logoutText);
    window.draw(spriteBtnClouse);

    window.draw(volumeBar);
    window.draw(btnVolume);
}