#pragma once
#include <deque>
#include <set>
#include <vector>
#include "UnitLoader.h" // ����� �������� ����������� UnitStats

// ������� ��� ������������� ������� ��������. 
// ��������� ����������� ��������� ������ � ���������� �, 
// ������������ �������� ������� set �������������� ��������.
std::deque<int> initializeDeck(const std::vector<UnitStats>& availableUnits, int numDeckCards, std::set<int>& usedIndices);

// ������� ��� ������ ��������� ��������, ������� ��� �� ������������.
int getNextCardIndex(const std::vector<UnitStats>& availableUnits, const std::set<int>& usedIndices);

