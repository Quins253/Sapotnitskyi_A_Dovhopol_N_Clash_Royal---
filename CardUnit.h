#pragma once
#include <string>

class CardUnit {
private:
    std::string name;
    int hp;
    float speed;
    int damage;
    int count;
    int cost;
    std::string imagePath;

public:
    CardUnit(const std::string& name, int hp, float speed, int damage, int count, int cost, const std::string& imagePath);

    std::string getName() const;
    int getHp() const;
    float getSpeed() const;
    int getDamage() const;
    int getCount() const;
    int getCost() const;
    std::string getImagePath() const;
};
