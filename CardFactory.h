#pragma once
#include "CardUnit.h"
#include <string>

class CardFactory {
public:
    static CardUnit* createCardFromLine(const std::string& line);
};
