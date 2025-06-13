// UserUnitManager.h
#pragma once
#include <string>
#include <vector>

class UserUnitManager {
public:
    static void createDefaultCardsFileIfNotExists(const std::string& username);
};
