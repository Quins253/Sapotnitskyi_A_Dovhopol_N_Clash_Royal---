#pragma once
#include <deque>
#include <set>
#include <vector>
#include "UnitLoader.h" // Чтобы получить определение UnitStats

// Функция для инициализации системы карточек. 
// Заполняет уникальными индексами колоду и возвращает её, 
// одновременно заполняя внешний set использованных индексов.
std::deque<int> initializeDeck(const std::vector<UnitStats>& availableUnits, int numDeckCards, std::set<int>& usedIndices);

// Функция для выбора следующей карточки, которая ещё не использована.
int getNextCardIndex(const std::vector<UnitStats>& availableUnits, const std::set<int>& usedIndices);

