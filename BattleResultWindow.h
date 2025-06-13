#pragma once
#include <SFML/Graphics.hpp>

class BattleResultWindow {
public:
    BattleResultWindow(bool win, int coins, int trophies);
    void show(sf::RenderWindow& window);

private:
    bool win;
    int coins;
    int trophies;
};
