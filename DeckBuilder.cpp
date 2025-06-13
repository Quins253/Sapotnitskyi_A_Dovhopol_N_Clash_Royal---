#include "DeckBuilder.h"
#include "CardUnit.h"
#include "CardManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

DeckBuilder::DeckBuilder(CardManager* cardManager, const std::string& username)
    : manager(cardManager), username(username) {
    font.loadFromFile("assets/fonts/CalSans-Regular.ttf");
    scrollOffsetY = 0.f;
    deckBaseY = 200.f;

    elixirIconTexture.loadFromFile("assets/icons/Elixir.png");
    elixirIconSprite.setTexture(elixirIconTexture);
    elixirIconSprite.setScale(0.5f, 0.5f);
    elixirText.setFont(font);
    elixirText.setCharacterSize(30);
    elixirText.setFillColor(sf::Color::White);
}

void DeckBuilder::setDeckSprite(const sf::Sprite& deckSprite) {
    this->deckPanelSprite = deckSprite;
    this->deckPanelSize = deckSprite.getGlobalBounds();
    updateCardSprites();
    updateLockedCardSprites();
    updateDeckCardSprites();
}

void DeckBuilder::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        scrollOffsetY -= event.mouseWheelScroll.delta * 30.f;
        if (scrollOffsetY < 0) scrollOffsetY = 0;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        
        if (showContextMenu) {
            sf::FloatRect addRect(contextMenuPosition.x, contextMenuPosition.y - scrollOffsetY, 200.f, 30.f);
            sf::FloatRect statsRect(contextMenuPosition.x, contextMenuPosition.y + 35.f - scrollOffsetY, 200.f, 30.f);

            if (addRect.contains(mousePos) && selectedCard) {
                if (manager->getDeckCards().size() < 8) {
                    addCardToDeck(selectedCard);
                }
                else {
                    isReplacingCard = true;
                    pendingCardToReplace = selectedCard;
                }
                showContextMenu = false;
                return;
            }
            else if (statsRect.contains(mousePos) && selectedCard) {
                showStatsWindow = !showStatsWindow;
                showContextMenu = false;
                return;
            }
            else {
                showContextMenu = false;
                return;
            }
        }

        if (isReplacingCard && pendingCardToReplace) {
            CardUnit* deckCard = getDeckCardFromClick(mousePos);
            if (deckCard) {
                removeCardFromDeck(deckCard);
                addCardToDeck(pendingCardToReplace);
                isReplacingCard = false;
                pendingCardToReplace = nullptr;
                return;
            }
        }

        CardUnit* deckCard = getDeckCardFromClick(mousePos);
        if (deckCard) {
            removeCardFromDeck(deckCard);
            return;
        }

        for (size_t i = 0; i < cardSprites.size(); ++i) {
            sf::FloatRect bounds = cardSprites[i].getGlobalBounds();
            bounds.top -= scrollOffsetY;
            if (bounds.contains(mousePos)) {
                contextMenuPosition = sf::Vector2f(mousePos.x, mousePos.y + scrollOffsetY);
                selectedCard = manager->getUnlockedCards()[i];
                showContextMenu = true;
                return;
            }
        }

        showContextMenu = false;
        showStatsWindow = false;
    }
}


CardUnit* DeckBuilder::getCardFromClick(const sf::Vector2f& mousePos) {
    for (size_t i = 0; i < cardSprites.size(); ++i) {
        sf::FloatRect bounds = cardSprites[i].getGlobalBounds();
        bounds.top -= scrollOffsetY;
        if (bounds.contains(mousePos)) {
            return manager->getUnlockedCards()[i];
        }
    }
    return nullptr;
}

CardUnit* DeckBuilder::getDeckCardFromClick(const sf::Vector2f& mousePos) {
    for (size_t i = 0; i < deckCardSprites.size(); ++i) {
        sf::FloatRect bounds = deckCardSprites[i].getGlobalBounds();
        bounds.top -= scrollOffsetY;
        if (bounds.contains(mousePos)) {
            return manager->getDeckCards()[i];
        }
    }
    return nullptr;
}

void DeckBuilder::addCardToDeck(CardUnit* card) {
    auto& deck = manager->getDeckCards();
    if (deck.size() >= 8) return;
    if (std::any_of(deck.begin(), deck.end(), [&](CardUnit* c) { return c->getName() == card->getName(); })) return;
    manager->addToDeck(card);
    auto& unlocked = manager->getUnlockedCards();
    unlocked.erase(std::remove_if(unlocked.begin(), unlocked.end(), [&](CardUnit* c) { return c->getName() == card->getName(); }), unlocked.end());
    updateUserUnitsFile();
    appendToDeckFile(card);
    updateCardSprites();
    updateDeckCardSprites();
}

void DeckBuilder::removeCardFromDeck(CardUnit* card) {
    auto& deck = manager->getDeckCards();
    deck.erase(std::remove_if(deck.begin(), deck.end(), [&](CardUnit* c) { return c->getName() == card->getName(); }), deck.end());
    manager->addToUnlocked(card);
    updateUserUnitsFile();
    updateDeckFile();
    updateCardSprites();
    updateDeckCardSprites();
}

void DeckBuilder::updateUserUnitsFile() {
    std::ofstream out("data/profiles/" + username + "_units.txt", std::ios::trunc);
    for (CardUnit* card : manager->getUnlockedCards()) {
        out << card->getName() << " " << card->getHp() << " " << card->getSpeed() << " "
            << card->getDamage() << " " << card->getCount() << " " << card->getCost() << " "
            << card->getImagePath() << "\n";
    }
    out.close();
}

void DeckBuilder::appendToDeckFile(CardUnit* card) {
    std::ofstream file("data/profiles/" + username + "_deck.txt", std::ios::app);
    file << card->getName() << " " << card->getHp() << " " << card->getSpeed() << " "
        << card->getDamage() << " " << card->getCount() << " " << card->getCost() << " "
        << card->getImagePath() << "\n";
    file.close();
}

void DeckBuilder::updateDeckFile() {
    std::ofstream out("data/profiles/" + username + "_deck.txt", std::ios::trunc);
    for (CardUnit* card : manager->getDeckCards()) {
        out << card->getName() << " " << card->getHp() << " " << card->getSpeed() << " "
            << card->getDamage() << " " << card->getCount() << " " << card->getCost() << " "
            << card->getImagePath() << "\n";
    }
    out.close();
}

void DeckBuilder::updateCardSprites() {
    
    cardSprites.clear();
    iconTextures.clear();
    
    const auto& cards = manager->getUnlockedCards();
    float cardWidth = 210.f, cardHeight = 270.f, spacing = 205.f;
    float scale = 0.22f;
    int cardsPerRow = 5;
    float secondRowOffsetY = 95.f;
    float windowWidth = 1280.f;
    float totalRowWidth = (cardWidth * scale) * cardsPerRow + spacing * (cardsPerRow - 1);
    float startX = (windowWidth - totalRowWidth) / 2.f + 165.f;
    float cardsStartY = deckPanelSprite.getPosition().y + deckPanelSize.height + 200.f;
    
    for (size_t i = 0; i < cards.size(); ++i) {
        
        CardUnit* card = cards[i];
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        
        if (!texture->loadFromFile(card->getImagePath())) continue;
        iconTextures.push_back(texture);
        
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setScale(scale, scale);
        
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float posX = startX + col * (cardWidth * scale + spacing);
        float posY = cardsStartY + row * (cardHeight * scale + spacing);
        
        if (row == 1) posY += secondRowOffsetY;
        sprite.setPosition(posX, posY);
        cardSprites.push_back(sprite);
    }
}

void DeckBuilder::updateDeckCardSprites() {
    
    deckCardSprites.clear();
    deckTextures.clear();
    
    const auto& deck = manager->getDeckCards();
    float cardWidth = 210.f, cardHeight = 270.f, spacing = 205.f;
    float scale = 0.22f;
    int cardsPerRow = 4;
    float startX = deckPanelSprite.getPosition().x + 231.f;
    float startY = deckPanelSprite.getPosition().y + 232.f;
    float secondRowOffsetY = 65.f;
    
    for (size_t i = 0; i < deck.size(); ++i) {
        CardUnit* card = deck[i];
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(card->getImagePath())) continue;
        deckTextures.push_back(texture);
        sf::Sprite sprite;
        sprite.setTexture(*texture);

        sf::Vector2u texSize = texture->getSize();
        float scaleX = cardWidth / static_cast<float>(texSize.x);
        float scaleY = cardHeight / static_cast<float>(texSize.y);
        sprite.setScale(scaleX, scaleY);
        
        int row = static_cast<int>(i) / cardsPerRow;
        int col = static_cast<int>(i) % cardsPerRow;
        float posX = startX + col * (cardWidth * scale + spacing);
        float posY = startY + row * (cardHeight * scale + spacing);
        
        if (row == 1) posY += secondRowOffsetY;
        
        sprite.setPosition(posX, posY);
        deckCardSprites.push_back(sprite);
    }
}

void DeckBuilder::updateLockedCardSprites() {
    
    lockedCardSprites.clear();
    lockedIconTextures.clear();
    
    const auto& lockedCards = manager->getLockedCards();
    float cardWidth = 210.f, cardHeight = 270.f, spacing = 45.f;
    float scale = 0.22f;;
    int cardsPerRow = 5;
    float secondRowOffsetY = 37.f;
    float windowWidth = 1380.f;
    float totalRowWidth = cardsPerRow * (cardWidth + spacing) - spacing;
    float startX = (windowWidth - totalRowWidth) / 2.f + 200.f;
    float cardsStartY = deckPanelSprite.getPosition().y + deckPanelSize.height + 930.f;
    
    for (size_t i = 0; i < lockedCards.size(); ++i) {
        
        CardUnit* card = lockedCards[i];
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(card->getImagePath())) continue;
        lockedIconTextures.push_back(texture);
        
        sf::Sprite sprite;
        sprite.setTexture(*texture);
        sprite.setScale(scale, scale);
        sprite.setColor(sf::Color(255, 255, 255, 100));
        int row = static_cast<int>(i) / cardsPerRow;
        int col = static_cast<int>(i) % cardsPerRow;
        float posX = startX + col * (cardWidth + spacing);
        float posY = cardsStartY + row * (cardHeight + spacing);
        
        if (row == 1) posY += secondRowOffsetY;
        
        sprite.setPosition(posX, posY);
        lockedCardSprites.push_back(sprite);
    }
}

float DeckBuilder::calculateAverageElixir() {
    const auto& deck = manager->getDeckCards();
    if (deck.empty()) return 0.f;

    float sum = 0.f;
    for (CardUnit* card : deck) {
        sum += card->getCost();
    }

    float average = sum / static_cast<float>(deck.size());

    return std::round(average * 10.f) / 10.f;
}

void DeckBuilder::render(sf::RenderWindow& window) {
    
    sf::Sprite deck = deckPanelSprite;
    
    float windowCenterX = window.getSize().x / 2.f;
    float panelOffsetX = windowCenterX - deckPanelSize.width / 2.f;
    float panelOffsetY = deckBaseY - scrollOffsetY;
    deck.setPosition(panelOffsetX, panelOffsetY);
    
    window.draw(deck);

    for (auto& sprite : deckCardSprites) {
        sf::Vector2f pos = sprite.getPosition();
        sprite.setPosition(pos.x, pos.y - scrollOffsetY);
        window.draw(sprite);
        sprite.setPosition(pos);
    }

    for (auto& sprite : cardSprites) {
        sf::Vector2f pos = sprite.getPosition();
        sprite.setPosition(pos.x, pos.y - scrollOffsetY);
        window.draw(sprite);
        sprite.setPosition(pos);
    }

    for (auto& sprite : lockedCardSprites) {
        sf::Vector2f pos = sprite.getPosition();
        sprite.setPosition(pos.x, pos.y - scrollOffsetY);
        window.draw(sprite);
        sprite.setPosition(pos);
    }

    if (showContextMenu) {
        sf::Vector2f menuPos = contextMenuPosition;
        menuPos.y -= scrollOffsetY;

        contextMenu.setPosition(menuPos);
        contextMenu.setSize(sf::Vector2f(200.f, 70.f));
        contextMenu.setFillColor(sf::Color(50, 50, 50, 230));
        window.draw(contextMenu);

        addToDeckText.setFont(font);
        addToDeckText.setString("Add Card In Deck");
        addToDeckText.setCharacterSize(16);
        addToDeckText.setFillColor(sf::Color::White);
        addToDeckText.setPosition(menuPos.x + 10.f, menuPos.y + 5.f);
        window.draw(addToDeckText);

        viewStatsText.setFont(font);
        viewStatsText.setString("Statistic");
        viewStatsText.setCharacterSize(16);
        viewStatsText.setFillColor(sf::Color::White);
        viewStatsText.setPosition(menuPos.x + 10.f, menuPos.y + 40.f);
        window.draw(viewStatsText);
    }

    if (showStatsWindow && selectedCard) {
        float boxWidth = 300.f;
        float boxHeight = 200.f;

        float posX = window.getSize().x - boxWidth - 20.f; 
        float posY = contextMenuPosition.y - scrollOffsetY; 

        if (posY + boxHeight > window.getSize().y)
            posY = window.getSize().y - boxHeight - 20.f;
        if (posY < 0)
            posY = 20.f;

        sf::RectangleShape statsBox;
        statsBox.setSize(sf::Vector2f(boxWidth, boxHeight));
        statsBox.setFillColor(sf::Color(0, 0, 0, 200));
        statsBox.setPosition(posX, posY);
        window.draw(statsBox);

        sf::Text stats;
        stats.setFont(font);
        stats.setCharacterSize(18);
        stats.setFillColor(sf::Color::White);
        stats.setPosition(posX + 10.f, posY + 10.f);
        stats.setString("name: " + selectedCard->getName() +
            "\nHP: " + std::to_string(selectedCard->getHp()) +
            "\nSpeed: " + std::to_string(selectedCard->getSpeed()) +
            "\nAttack: " + std::to_string(selectedCard->getDamage()) +
            "\nHow Many: " + std::to_string(selectedCard->getCount()) +
            "\nCost: " + std::to_string(selectedCard->getCost()));
        window.draw(stats);
    }
    if (!deckCardSprites.empty()) {
        float baseX = deckPanelSprite.getPosition().x + 650.f;
        float baseY = deckPanelSprite.getPosition().y + 950.f;

        float scrollY = baseY - scrollOffsetY;

        elixirIconSprite.setPosition(baseX, scrollY);
        elixirText.setString(std::to_string(calculateAverageElixir()).substr(0, 4));
        elixirText.setPosition(baseX + 50.f, scrollY + 5.f);

        window.draw(elixirIconSprite);
        window.draw(elixirText);
    }

}

