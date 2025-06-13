#pragma once
#include "Utils.h"
#include <cmath>
#include <algorithm>

sf::Vector2f getTileCenter(int x, int y) {
    return sf::Vector2f(static_cast<float>(x * tileSize) + tileSize / 2.f,
        static_cast<float>(y * tileSize) + tileSize / 2.f);
}

float distance(sf::Vector2f a, sf::Vector2f b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) +
        (a.y - b.y) * (a.y - b.y));
}

sf::Vector2f rotateVector(const sf::Vector2f& v, float angle) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return sf::Vector2f(v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA);
}

sf::Vector2f getApproachPointOnRect(const sf::FloatRect& rect, const sf::Vector2f& unitPos) {
    const float offset = 5.f; // отступ внутрь башни
    sf::Vector2f center(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
    sf::Vector2f candidate;
    float diffX = unitPos.x - center.x;
    float diffY = unitPos.y - center.y;

    if (std::abs(diffX) > std::abs(diffY)) {
        candidate.x = (diffX < 0) ? rect.left + offset : rect.left + rect.width - offset;
        candidate.y = std::min(std::max(unitPos.y, rect.top + offset), rect.top + rect.height - offset);
    }
    else {
        candidate.y = (diffY < 0) ? rect.top + offset : rect.top + rect.height - offset;
        candidate.x = std::min(std::max(unitPos.x, rect.left + offset), rect.left + rect.width - offset);
    }
    return candidate;
}

sf::Vector2f getTowerTargetPoint(const sf::FloatRect& rect, const sf::Vector2f& unitPos, float attackRange) {
    if (attackRange > 120.f) {
        // ƒл€ дальнобойных юнитов возвращаем центр башни
        return sf::Vector2f(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
    }
    else {
        // ƒл€ ближнего бо€ Ч точка на границе башни
        return getApproachPointOnRect(rect, unitPos);
    }
}

bool isPositionAllowed(sf::Vector2f pos,
    const std::vector<Tile>& tiles,
    const std::vector<Tile>& bridges,
    const std::vector<Tower>& towers,
    const std::vector<Tower>& friendlyTowers)
{
    int tx = static_cast<int>(pos.x) / tileSize;
    int ty = static_cast<int>(pos.y) / tileSize;
    if (tx < 0 || tx >= tilesX || ty < 0 || ty >= tilesY)
        return false;
    int idx = ty * tilesX + tx;
    if (tiles[idx].type == River) {
        for (auto& bridge : bridges) {
            if (bridge.shape.getGlobalBounds().contains(pos))
                return true;
        }
        return false;
    }
    for (auto& tower : towers)
        if (tower.shape.getGlobalBounds().contains(pos))
            return false;
    for (auto& tower : friendlyTowers)
        if (tower.shape.getGlobalBounds().contains(pos))
            return false;
    return true;
}
