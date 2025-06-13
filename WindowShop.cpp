#include "WindowShop.h"
#include "MenuButton.h"
#include "MainMenu.h"
#include "WindowDeck.h"
#include "CardManager.h"
#include "CardUnit.h"
#include "ManagerProfile.h"
#include <memory>

#include <vector>
#include <SFML/Graphics.hpp>

void showShopWindow(sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();

    // bg
    sf::Texture textureBackground;
    textureBackground.loadFromFile("assets/images/Wallpaper Clash Royale PC.jpeg");
    sf::Sprite backgroundSprite(textureBackground);
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureBackground.getSize().x,
        static_cast<float>(windowSize.y) / textureBackground.getSize().y
    );

    // banner
    sf::Texture textureBanner;
    textureBanner.loadFromFile("assets/icons/Banner_Shop.png");
    sf::Sprite spriteBanner(textureBanner);
    spriteBanner.setScale(0.5f, 0.5f);
    spriteBanner.setPosition(
        (window.getSize().x - spriteBanner.getGlobalBounds().width) / 2.f,
        50.f
    );

    // Bottom Buttons
    sf::Texture textureButton1, textureButton2, textureButton3;
    textureButton1.loadFromFile("assets/icons/Cards.png");
    textureButton2.loadFromFile("assets/icons/battle1.png");
    textureButton3.loadFromFile("assets/icons/ShopIcon.png");

    std::vector<MenuButton> buttons;
    buttons.emplace_back(&textureButton1, sf::Vector2f(820.f, 950.f), sf::Vector2f(80.f, 80.f));
    buttons.emplace_back(&textureButton2, sf::Vector2f(930.f, 950.f), sf::Vector2f(80.f, 80.f));
    buttons.emplace_back(&textureButton3, sf::Vector2f(1040.f, 950.f), sf::Vector2f(80.f, 80.f));

    // Bottom Line
    sf::Texture bottomLine;
    bottomLine.loadFromFile("assets/images/bottomLine.png");

    sf::Sprite spriteBottomLine;
    spriteBottomLine.setTexture(bottomLine);
    spriteBottomLine.setScale(0.3f, 0.5f);
    spriteBottomLine.setOrigin(bottomLine.getSize().x / 2.f, bottomLine.getSize().y / 2.f);
    spriteBottomLine.setPosition(windowSize.x / 2.f, 990.f);

    std::string username = profileManager::getActiveUser();
    CardManager cardManager;
    cardManager.loadUserCards(username);
    cardManager.loadAllCards("data/cards/all_cards.txt");

    std::vector<CardUnit*> lockedCards = cardManager.getLockedCards();

    // Card
    std::vector<std::shared_ptr<sf::Texture>> cardTextures;
    std::vector<sf::Sprite> cardSprites;

    const float startX = 100.f;
    const float startY = 250.f;
    const float spacingX = 150.f;
    const float spacingY = 180.f;
    const int cardsPerRow = 5;

    for (size_t i = 0; i < lockedCards.size(); ++i) {
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        if (texture->loadFromFile(lockedCards[i]->getImagePath())) {
            sf::Sprite sprite(*texture);
            float x = startX + (i % cardsPerRow) * spacingX;
            float y = startY + (i / cardsPerRow) * spacingY;
            sprite.setPosition(x, y);
            sprite.setScale(0.6f, 0.6f);

            cardTextures.push_back(texture);
            cardSprites.push_back(sprite);
        }
        else {
            std::cerr << "Faild Card: " << lockedCards[i]->getName() << std::endl;
        }
    }

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                return;
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(spriteBanner);
        window.draw(spriteBottomLine);

        for (int i = 0; i < buttons.size(); ++i) {
            buttons[i].setHoverEffect(buttons[i].isMouseOver(window));
            buttons[i].draw(window);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttons[i].isMouseOver(window)) {
                if (i == 0) {
                    string username = profileManager::getActiveUser();
                    showDeckWindow(window, username); // Cards
                }
                else if (i == 1) {
                    showMainMenu(window); // Battle
                }
                else if (i == 2) {
                    // Shop
                }
            }
        }

        for (auto& sprite : cardSprites) {
            window.draw(sprite);
        }

        window.display();
    }
}
