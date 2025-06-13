#pragma once
#include "CardSystem.h"
#include <cstdlib> 

std::deque<int> initializeDeck(const std::vector<UnitStats>& availableUnits, int numDeckCards, std::set<int>& usedIndices) {
    std::deque<int> deckIndices;
    while (usedIndices.size() < static_cast<size_t>(numDeckCards)) {
        int randomIndex = rand() % availableUnits.size();
        usedIndices.insert(randomIndex);
    }
    for (auto idx : usedIndices)
        deckIndices.push_back(idx);
    return deckIndices;
}

int getNextCardIndex(const std::vector<UnitStats>& availableUnits, const std::set<int>& usedIndices) {
    int nextCardIndex;
    do {
        nextCardIndex = rand() % availableUnits.size();
    } while (usedIndices.find(nextCardIndex) != usedIndices.end());
    return nextCardIndex;
}
