// UserUnitManager.cpp
#include "UserUnitManager.h"
#include <fstream>
#include <filesystem>

static const std::vector<std::string> defaultCardData = {
    "Wizard 155 0.5 0 1 5 assets/Units_img/WizardCard.png",
    "Cannon 520 0 0 1 6 assets/Units_img/CanonCard.png",
    "Archer 130 0.65 200 1 4 assets/Units_img/BowmanCard.png",
    "Berserker 110 0.9 128 1 2 assets/Units_img/BerserkCard.png",
    "Gigant 650 0.25 150 1 7 assets/Units_img/GigantCard.png",
    "Assassin 95 1.1 50 1 4 assets/Units_img/KillerCard.png",
    "PEKKA 210 0.6 150 1 3 assets/Units_img/MiniPEKKACard.png",
    "Inferno 520 0 0 1 5 assets/Units_img/InfernoCard.png",
    "Dark_Archer 125 0.7 0 1 4 assets/Units_img/Dark_ArcherCard.png"
};

void UserUnitManager::createDefaultCardsFileIfNotExists(const std::string& username) {
    std::string unitPath = "data/profiles/" + username + "_units.txt";
    std::string deckPath = "data/profiles/" + username + "_deck.txt";

    if (!std::filesystem::exists(unitPath)) {
        std::ofstream file(unitPath);
        for (const std::string& cardLine : defaultCardData)
            file << cardLine << "\n";
    }

    if (!std::filesystem::exists(deckPath)) {
        std::ofstream(deckPath);
    }
}
