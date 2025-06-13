#include "WindowDeck.h"
#include "DeckBuilder.h"
#include "CardManager.h"
#include "CardFactory.h"
#include "ManagerProfile.h"
#include "UserUnitManager.h"
#include "MenuButton.h"
#include "MainMenu.h"
#include "WindowShop.h"

void showDeckWindow(sf::RenderWindow& window, const std::string& username) {

    sf::Vector2u windowSize = window.getSize();
    
    sf::Texture textureBackground;
    textureBackground.loadFromFile("assets/images/Wallpaper Clash Royale PC.jpeg");
    sf::Sprite backgroundSprite(textureBackground);
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureBackground.getSize().x,
        static_cast<float>(windowSize.y) / textureBackground.getSize().y
    );

    
    sf::Texture textureDeck;
    textureDeck.loadFromFile("assets/images/Deck.jpg");
    sf::Sprite spriteDeck(textureDeck);
    spriteDeck.setScale(0.5f, 0.5f);
    spriteDeck.setPosition(
        (window.getSize().x - spriteDeck.getGlobalBounds().width) / 2.f,
        50.f
    );

    //Bottom Buttons
    sf::Texture textureButton1, textureButton2, textureButton3;
    textureButton1.loadFromFile("assets/icons/Cards.png");
    textureButton2.loadFromFile("assets/icons/battle1.png");
    textureButton3.loadFromFile("assets/icons/ShopIcon.png");

    vector<MenuButton> buttons;
    buttons.emplace_back(&textureButton1, sf::Vector2f(820.f, 950.f), sf::Vector2f(80.f, 80.f));
    buttons.emplace_back(&textureButton2, sf::Vector2f(930.f, 950.f), sf::Vector2f(80.f, 80.f));
    buttons.emplace_back(&textureButton3, sf::Vector2f(1040.f, 950.f), sf::Vector2f(80.f, 80.f));

    //Bottom Line for bottom buttons

    sf::Texture bottomLine;
    bottomLine.loadFromFile("assets/images/bottomLine.png");

    sf::Sprite spriteBottomLine;
    spriteBottomLine.setTexture(bottomLine);
    spriteBottomLine.setScale(0.3f, 0.5f);
    spriteBottomLine.setOrigin(bottomLine.getSize().x / 2.f, bottomLine.getSize().y / 2.f);
    spriteBottomLine.setPosition(windowSize.x / 2.f, 990.f);

    
    UserUnitManager::createDefaultCardsFileIfNotExists(username);

    CardManager cardManager;
    cardManager.loadAllCards("data/all_cards.txt");
    cardManager.loadUserCards(username);    

    DeckBuilder builder(&cardManager, username);
    builder.setDeckSprite(spriteDeck);  

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                return;

            builder.handleEvent(event, sf::Vector2f(sf::Mouse::getPosition(window)));
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(spriteBottomLine);

        for (int i = 0; i < buttons.size(); ++i) {
            buttons[i].setHoverEffect(buttons[i].isMouseOver(window));
            buttons[i].draw(window);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttons[i].isMouseOver(window)) {
                if (i == 0) {
                    return;
                }
                else if (i == 1) {
                    showMainMenu(window);
                }
                else if (i == 2) {
                    showShopWindow(window);
                }
            }
        }
        builder.render(window);
        window.display();
    }
}
