#pragma once
#include "CardUnit.h"
#include <vector>
#include <string>

class CardManager {
private:
    std::vector<CardUnit*> allCards;
    std::vector<CardUnit*> unlockedCards;
    std::vector<CardUnit*> deckCards;

public:
    void loadUserCards(const std::string& username);
    void loadAllCards(const std::string& path);

    void addToAll(CardUnit* card);
    void addToUnlocked(CardUnit* card);
    void addToDeck(CardUnit* card);
    void removeFromDeck(CardUnit* card);

    std::vector<CardUnit*>& getAllCards();
    std::vector<CardUnit*>& getUnlockedCards();
    std::vector<CardUnit*>& getDeckCards();

    std::vector<CardUnit*> getLockedCards() const;
};
