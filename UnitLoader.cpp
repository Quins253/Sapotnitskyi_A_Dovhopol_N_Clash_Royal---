#pragma once
#include "UnitLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::map<std::string, sf::Texture> textureCache;

std::vector<UnitStats> loadUnitStats(const std::string& filename) {
    std::vector<UnitStats> stats;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return stats;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        UnitStats unit;
        int r, g, b;

        if (iss >> unit.name >> unit.health >> unit.speed >> unit.radius
            >> r >> g >> b
            >> unit.damage >> unit.attackCooldown >> unit.attackRange
            >> unit.agroRange >> unit.manaCost) {
            unit.color = sf::Color(r, g, b);

            std::string texturePath;
            if (iss >> texturePath)
                unit.texturePath = texturePath;
            else
                unit.texturePath = "";
            std::string splashArt;
            if (iss >> splashArt)
                unit.splashArtPath = splashArt;
            else
                unit.splashArtPath = ""; 
            stats.push_back(unit);

            stats.push_back(unit);
        }
        else {
            std::cerr << "Ошибка парсинга строки: " << line << std::endl;
        }
    }

    return stats;
}

Unit createUnitFromStats(const UnitStats& stats, sf::Vector2f spawnPosition) {
    Unit unit;
    unit.stats = stats;
    unit.health = stats.health;
    unit.maxHealth = stats.health;
    unit.attackCooldown = stats.attackCooldown; 
    unit.isEnemy = false; 

    unit.shape.setRadius(stats.radius);
    unit.shape.setOrigin(stats.radius, stats.radius);
    unit.shape.setPosition(spawnPosition);

    unit.shape.setFillColor(stats.color);

    unit.healthBar.setSize({ 20.f, 5.f });
    unit.healthBar.setFillColor(sf::Color::Green);
    unit.healthBar.setOrigin(10.f, 25.f); 

    if (!stats.texturePath.empty()) {
        if (textureCache.find(stats.texturePath) == textureCache.end()) {
            sf::Texture texture;
            if (!texture.loadFromFile(stats.texturePath)) {
                std::cerr << "Ошибка загрузки текстуры: " << stats.texturePath << std::endl;
            }
            else {
                texture.setSmooth(true);
                textureCache[stats.texturePath] = texture;
            }

        }
        if (textureCache.find(stats.texturePath) != textureCache.end()) {
            unit.shape.setFillColor(sf::Color::White);
            unit.shape.setTexture(&textureCache[stats.texturePath]);
            sf::IntRect texRect(0, 0,
                textureCache[stats.texturePath].getSize().x,
                textureCache[stats.texturePath].getSize().y);
            unit.shape.setTextureRect(texRect);
        }
    }

    return unit;
}
