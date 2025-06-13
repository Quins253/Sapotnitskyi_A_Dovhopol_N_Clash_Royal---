#include "CardUnit.h"

CardUnit::CardUnit(const std::string& name, int hp, float speed, int damage, int count, int cost, const std::string& imagePath)
    : name(name), hp(hp), speed(speed), damage(damage), count(count), cost(cost), imagePath(imagePath) {}

std::string CardUnit::getName() const { return name; }
int CardUnit::getHp() const { return hp; }
float CardUnit::getSpeed() const { return speed; }
int CardUnit::getDamage() const { return damage; }
int CardUnit::getCount() const { return count; }
int CardUnit::getCost() const { return cost; }
std::string CardUnit::getImagePath() const { return imagePath; }
