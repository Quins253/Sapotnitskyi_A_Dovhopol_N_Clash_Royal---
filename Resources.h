#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// Функции для загрузки ресурсов (шрифтов, текстур и т.д.)
bool loadFont(sf::Font& font, const std::string& filePath);
bool loadTexture(sf::Texture& texture, const std::string& filePath);


