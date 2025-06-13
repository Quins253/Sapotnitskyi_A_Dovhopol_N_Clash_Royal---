#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum TileType {
    Grass,
    River,
    Bridge,
    Fence
};

struct Tile {
    sf::RectangleShape shape;
    TileType type;
};

struct UnitStats {
    std::string name;
    float health;
    float speed;
    float radius;
    sf::Color color;
    float damage;
    float attackCooldown;
    float attackRange;
    float agroRange;
    float manaCost;
    std::string texturePath;
    std::string splashArtPath;
};

struct Unit {
    sf::CircleShape shape;
    sf::RectangleShape healthBar;
    sf::Vector2f target;
    bool crossedRiver = false;
    float health = 100;
    float maxHealth = 100;
    float attackCooldown = 0;
    UnitStats stats;
    bool isEnemy = false;
    Unit* enemyTarget = nullptr;
};

struct Tower {
    sf::RectangleShape shape;
    sf::RectangleShape healthBar;
    float health;
    float maxHealth;
    float attackCooldown = 0;
};

struct Projectile {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float damage;
    Unit* target;
    sf::Vector2f staticTarget;
};

struct UnitProjectile {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float damage;
    Tower* target;
};