#pragma once
#include "Resources.h"
#include <iostream>

bool loadFont(sf::Font& font, const std::string& filePath) {
    if (!font.loadFromFile(filePath)) {
        std::cerr << "������ �������� ������: " << filePath << std::endl;
        return false;
    }
    return true;
}

bool loadTexture(sf::Texture& texture, const std::string& filePath) {
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "�� ������� ��������� ��������: " << filePath << std::endl;
        return false;
    }
    return true;
}
