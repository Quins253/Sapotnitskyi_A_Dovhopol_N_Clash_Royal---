#include "CardFactory.h"
#include <sstream>

CardUnit* CardFactory::createCardFromLine(const std::string& line) {
    std::istringstream iss(line);
    std::string name, imagePath;
    int hp, damage, count, cost;
    float speed;

    if (!(iss >> name >> hp >> speed >> damage >> count >> cost >> imagePath)) {
        return nullptr;
    }

    return new CardUnit(name, hp, speed, damage, count, cost, imagePath);
}
