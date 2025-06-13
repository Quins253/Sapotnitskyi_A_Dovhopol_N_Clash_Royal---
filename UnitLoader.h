#pragma once
#include <vector>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"

// Загружает характеристики юнитов из файла.
// Файл должен содержать строки с данными, разделёнными пробелами.
std::vector<UnitStats> loadUnitStats(const std::string& filename);

// Создаёт юнита на основе характеристик и заданной позиции спауна.
Unit createUnitFromStats(const UnitStats& stats, sf::Vector2f spawnPosition);

// Глобальный кэш для хранения загруженных текстур.
extern std::map<std::string, sf::Texture> textureCache;

