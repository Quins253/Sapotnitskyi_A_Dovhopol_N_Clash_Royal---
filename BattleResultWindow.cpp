#include "BattleResultWindow.h"
#include <string>
#include <iostream>

using namespace std;

BattleResultWindow::BattleResultWindow(bool win, int coins, int trophies)
    : win(win), coins(coins), trophies(trophies) {}

void BattleResultWindow::show(sf::RenderWindow& window) {
    std::string imagePath = win ? "assets/images/battle_result__v.png" : "assets/images/battle_result__d.png";

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile(imagePath)) {
        cout << "Failed to load background image: " << imagePath << std::endl;
        return;
    }

    sf::Sprite bgSprite(bgTexture);
    bgSprite.setScale(
        window.getSize().x / static_cast<float>(bgTexture.getSize().x),
        window.getSize().y / static_cast<float>(bgTexture.getSize().y)
    );

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/CalSans-Regular.ttf")) {
        cout << "Failed to load font!\n";
        return;
    }

    sf::Text coinsText, trophiesText;
    coinsText.setFont(font);
    trophiesText.setFont(font);

    coinsText.setString("Coins: + " + to_string(coins));
    coinsText.setCharacterSize(28);
    coinsText.setFillColor(sf::Color::Yellow);
    coinsText.setPosition(window.getSize().x / 2.f - 70.f, 540.f);

    trophiesText.setString("Trophies: " + string(win ? "+ " : "- ") + to_string(trophies));
    trophiesText.setCharacterSize(28);
    trophiesText.setFillColor(sf::Color::White);
    trophiesText.setPosition(window.getSize().x / 2.f - 70.f, 590.f);

    sf::Clock timer;
    while (timer.getElapsedTime().asSeconds() < 3.f) {
        window.clear();
        window.draw(bgSprite);
        window.draw(coinsText);
        window.draw(trophiesText);
        window.display();
    }
}
