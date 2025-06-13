#include "CardManager.h"
#include "CardFactory.h"
#include <fstream>
#include <iostream>
#include <unordered_set>

void CardManager::loadUserCards(const std::string& username) {
    unlockedCards.clear();
    deckCards.clear();

    std::ifstream unlockedFile("data/profiles/" + username + "_units.txt");
    std::ifstream deckFile("data/profiles/" + username + "_deck.txt");

    std::string line;
    while (std::getline(unlockedFile, line)) {
        CardUnit* card = CardFactory::createCardFromLine(line);
        if (card) unlockedCards.push_back(card);
    }

    while (std::getline(deckFile, line)) {
        CardUnit* card = CardFactory::createCardFromLine(line);
        if (card) deckCards.push_back(card);
    }
}

void CardManager::loadAllCards(const std::string& path) {
    allCards.clear();

    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        CardUnit* card = CardFactory::createCardFromLine(line);
        if (card) allCards.push_back(card);
    }
}

void CardManager::addToAll(CardUnit* card) {
    allCards.push_back(card);
}

void CardManager::addToUnlocked(CardUnit* card) {
    unlockedCards.push_back(card);
}

void CardManager::addToDeck(CardUnit* card) {
    deckCards.push_back(card);
}

void CardManager::removeFromDeck(CardUnit* card) {
    deckCards.erase(std::remove_if(deckCards.begin(), deckCards.end(),
        [&](CardUnit* c) { return c->getName() == card->getName(); }),
        deckCards.end());
}

std::vector<CardUnit*>& CardManager::getAllCards() {
    return allCards;
}

std::vector<CardUnit*>& CardManager::getUnlockedCards() {
    return unlockedCards;
}

std::vector<CardUnit*>& CardManager::getDeckCards() {
    return deckCards;
}

std::vector<CardUnit*> CardManager::getLockedCards() const {
    std::unordered_set<std::string> unlockedNames;
    for (const auto& card : unlockedCards) {
        unlockedNames.insert(card->getName());
    }

    std::unordered_set<std::string> deckNames;
    for (const auto& card : deckCards) {
        deckNames.insert(card->getName());
    }

    std::vector<CardUnit*> locked;
    for (auto* card : allCards) {
        const std::string& name = card->getName();
        if (unlockedNames.find(name) == unlockedNames.end() &&
            deckNames.find(name) == deckNames.end()) {
            locked.push_back(card);
        }
    }

    return locked;
}
