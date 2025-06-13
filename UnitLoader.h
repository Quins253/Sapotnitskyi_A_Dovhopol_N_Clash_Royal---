#pragma once
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"

// ��������� �������������� ������ �� �����.
// ���� ������ ��������� ������ � �������, ����������� ���������.
std::vector<UnitStats> loadUnitStats(const std::string& filename);

// ������ ����� �� ������ ������������� � �������� ������� ������.
Unit createUnitFromStats(const UnitStats& stats, sf::Vector2f spawnPosition);

// ���������� ��� ��� �������� ����������� �������.
extern std::map<std::string, sf::Texture> textureCache;

