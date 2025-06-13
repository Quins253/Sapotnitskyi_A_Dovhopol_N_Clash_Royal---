#pragma once


#include <SFML/Graphics.hpp>
#include "constants.h"  // ���������� ���������
#include "GameObjects.h"

// ���������� ����� ����� �� ����������� (x, y)
sf::Vector2f getTileCenter(int x, int y);

// ��������� ���������� ���������� ����� ����� �������
float distance(sf::Vector2f a, sf::Vector2f b);

// ������������ ������ v �� ���� angle (� ��������)
sf::Vector2f rotateVector(const sf::Vector2f& v, float angle);

// ��������� ����� �� ������� ��������������, �������� ������������ � ������� unitPos
sf::Vector2f getApproachPointOnRect(const sf::FloatRect& rect, const sf::Vector2f& unitPos);

// ���������� ������� ����� ��� ����� �����, � ����������� �� ��������� �����
sf::Vector2f getTowerTargetPoint(const sf::FloatRect& rect, const sf::Vector2f& unitPos, float attackRange);

bool isPositionAllowed(sf::Vector2f pos,
    const std::vector<Tile>& tiles,
    const std::vector<Tile>& bridges,
    const std::vector<Tower>& towers,
    const std::vector<Tower>& friendlyTowers);
