#pragma once


#include <SFML/Graphics.hpp>
#include "constants.h"  // »спользуем константы
#include "GameObjects.h"

// ¬озвращает центр тайла по координатам (x, y)
sf::Vector2f getTileCenter(int x, int y);

// ¬ычисл€ет евклидовое рассто€ние между двум€ точками
float distance(sf::Vector2f a, sf::Vector2f b);

// ѕоворачивает вектор v на угол angle (в радианах)
sf::Vector2f rotateVector(const sf::Vector2f& v, float angle);

// ¬ычисл€ет точку на стороне пр€моугольника, наиболее приближенную к позиции unitPos
sf::Vector2f getApproachPointOnRect(const sf::FloatRect& rect, const sf::Vector2f& unitPos);

// ¬озвращает целевую точку дл€ атаки башни, в зависимости от диапазона атаки
sf::Vector2f getTowerTargetPoint(const sf::FloatRect& rect, const sf::Vector2f& unitPos, float attackRange);

bool isPositionAllowed(sf::Vector2f pos,
    const std::vector<Tile>& tiles,
    const std::vector<Tile>& bridges,
    const std::vector<Tower>& towers,
    const std::vector<Tower>& friendlyTowers);
