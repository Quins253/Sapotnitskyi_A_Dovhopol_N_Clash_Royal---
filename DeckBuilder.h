//#pragma once
//
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <memory>
//#include <string>
//
//class CardUnit;
//class CardManager;
//
//class DeckBuilder {
//public:
//    DeckBuilder(CardManager* cardManager, const std::string& username);
//    void setDeckSprite(const sf::Sprite& deckSprite);
//    void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos);
//    void render(sf::RenderWindow& window);
//
//private:
//    CardManager* manager;
//    std::string username;
//
//    sf::Sprite deckPanelSprite;
//    sf::FloatRect deckPanelSize;
//    float scrollOffsetY;
//    float deckBaseY;
//
//    sf::Font font;
//
//    std::vector<sf::Sprite> cardSprites;
//    std::vector<std::shared_ptr<sf::Texture>> iconTextures;
//
//    std::vector<sf::Sprite> deckCardSprites;
//    std::vector<std::shared_ptr<sf::Texture>> deckTextures;
//
//    std::vector<sf::Sprite> lockedCardSprites;
//    std::vector<std::shared_ptr<sf::Texture>> lockedIconTextures;
//
//    sf::RectangleShape contextMenu;
//    sf::Text addToDeckText;
//    sf::Text viewStatsText;
//    bool showContextMenu = false;
//    sf::Vector2f contextMenuPosition;
//
//    bool showStatsWindow = false;
//    CardUnit* selectedCard = nullptr;
//
//    CardUnit* pendingCardToReplace = nullptr;
//    bool isReplacingCard = false;
//
//    CardUnit* getCardFromClick(const sf::Vector2f& mousePos);
//    CardUnit* getDeckCardFromClick(const sf::Vector2f& mousePos);
//
//    void addCardToDeck(CardUnit* card);
//    void removeCardFromDeck(CardUnit* card);
//
//    void updateUserUnitsFile();
//    void appendToDeckFile(CardUnit* card);
//    void updateDeckFile();
//
//    void updateCardSprites();
//    void updateDeckCardSprites();
//    void updateLockedCardSprites();
//};


#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

class CardUnit;
class CardManager;

class DeckBuilder {
public:
    DeckBuilder(CardManager* cardManager, const std::string& username);

    void setDeckSprite(const sf::Sprite& deckSprite);
    void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos);
    void render(sf::RenderWindow& window);

private:
    CardManager* manager;
    std::string username;

    sf::Sprite deckPanelSprite;
    sf::FloatRect deckPanelSize;

    std::vector<sf::Sprite> cardSprites;
    std::vector<sf::Sprite> deckCardSprites;
    std::vector<sf::Sprite> lockedCardSprites;
    std::vector<std::shared_ptr<sf::Texture>> iconTextures;
    std::vector<std::shared_ptr<sf::Texture>> deckTextures;
    std::vector<std::shared_ptr<sf::Texture>> lockedIconTextures;

    sf::Font font;

    float scrollOffsetY;
    float deckBaseY;

    bool showContextMenu = false;
    sf::Vector2f contextMenuPosition;
    sf::RectangleShape contextMenu;
    sf::Text addToDeckText;
    sf::Text viewStatsText;

    bool showStatsWindow = false;
    CardUnit* selectedCard = nullptr;

    bool isReplacingCard = false;
    CardUnit* pendingCardToReplace = nullptr;

    sf::Texture elixirIconTexture;
    sf::Sprite elixirIconSprite;
    sf::Text elixirText;

    CardUnit* getCardFromClick(const sf::Vector2f& mousePos);
    CardUnit* getDeckCardFromClick(const sf::Vector2f& mousePos);

    void addCardToDeck(CardUnit* card);
    void removeCardFromDeck(CardUnit* card);

    void updateUserUnitsFile();
    void appendToDeckFile(CardUnit* card);
    void updateDeckFile();

    void updateCardSprites();
    void updateDeckCardSprites();
    void updateLockedCardSprites();

    float calculateAverageElixir();
};