#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// ������� ��� �������� �������� (�������, ������� � �.�.)
bool loadFont(sf::Font& font, const std::string& filePath);
bool loadTexture(sf::Texture& texture, const std::string& filePath);


