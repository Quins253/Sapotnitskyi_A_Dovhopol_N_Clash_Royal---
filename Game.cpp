#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <limits>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <set>
#include <map>
#include "Constants.h"
#include "Utils.h"
#include "GameObjects.h"
#include "UnitLoader.h"
#include "CardSystem.h"
#include "Resources.h"

void startGame(sf::RenderWindow& window, const std::string& username)() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::vector<UnitStats> availableUnits = loadUnitStats("data/ /chary.txt");
    if (availableUnits.empty()) {
        std::cerr << "Нет загруженных характеристик юнитов. Завершение программы." << std::endl;
        return;
    }

    if (textureCache.find("grassTexture") == textureCache.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile("grass_texture.png")) {
            std::cerr << "Error for grass!" << std::endl;
        }
        else {
            texture.setSmooth(true);
            textureCache["grassTexture"] = texture;
        }
    }
    if (textureCache.find("riverTexture") == textureCache.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile("rivertexture.png")) {
            std::cerr << "Error for river!" << std::endl;
        }
        else {
            texture.setSmooth(true);
            textureCache["riverTexture"] = texture;
        }
    }
    if (textureCache.find("bridgeTexture") == textureCache.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile("bridgetexture.png")) {
            std::cerr << "Error for bridge!" << std::endl;
        }
        else {
            texture.setSmooth(true);
            textureCache["bridgeTexture"] = texture;
            textureCache["bridgeTexture"].setRepeated(true);
        }
    }
    if (textureCache.find("towerTexture") == textureCache.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile("tower_texture.png")) {
            std::cerr << "Error for Tower!" << std::endl;
        }
        else {
            texture.setSmooth(true);  // сглаживание, если нужно
            textureCache["towerTexture"] = texture;
        }
    }



    std::deque<int> deckIndices;
    const int numDeckCards = 4;
    std::set<int> usedIndices;
    while (usedIndices.size() < numDeckCards) {
        int randomIndex = rand() % availableUnits.size();
        usedIndices.insert(randomIndex);
    }
    for (auto idx : usedIndices)
        deckIndices.push_back(idx);

    int nextCardIndex;
    do {
        nextCardIndex = rand() % availableUnits.size();
    } while (usedIndices.find(nextCardIndex) != usedIndices.end());

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Sqrt Royale", sf::Style::Fullscreen);
    window.setFramerateLimit(60);


    float gameWidth = windowWidth;
    float gameHeight = windowHeight;

    float leftMargin = (desktopMode.width - gameWidth) / 2.f;
    float topMargin = (desktopMode.height - gameHeight) / 2.f;

    float viewportLeft = leftMargin / desktopMode.width;
    float viewportTop = topMargin / desktopMode.height;
    float viewportWidth = gameWidth / desktopMode.width;
    float viewportHeight = gameHeight / desktopMode.height;

    sf::View gameView(sf::FloatRect(0, 0, gameWidth, gameHeight));
    gameView.setViewport(sf::FloatRect(viewportLeft, viewportTop, viewportWidth, viewportHeight));

    sf::Texture sideTexture;
    if (!sideTexture.loadFromFile("side_background.jpg")) {

    }
    sf::Sprite backgroundSprite(sideTexture);

    backgroundSprite.setScale(
        desktopMode.width / sideTexture.getSize().x,
        desktopMode.height / sideTexture.getSize().y
    );

    int enemyCrowns = 0;
    int crowns = 0;
    bool gameOver = false;

    sf::Color grassColor(100, 200, 100);
    sf::Color riverColor(50, 150, 255);
    sf::Color bridgeColor(139, 69, 19);
    sf::Color fenceColor(150, 75, 0);
    sf::Color towerColor(200, 0, 0);

    std::vector<Tile> tiles;
    std::vector<Tile> bridges;

    int riverY1 = tilesY / 2 - 1;
    int riverY2 = tilesY / 2;

    std::vector<sf::Vector2i> bridgeCoords = { {3, riverY1}, {14, riverY1} };

    for (int y = 0; y < tilesY; ++y) {
        for (int x = 0; x < tilesX; ++x) {
            Tile tile;
            tile.shape.setSize(sf::Vector2f(tileSize - 1, tileSize - 1));
            tile.shape.setPosition(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize));
            tile.type = Grass;

            // Используем текстуру травы для типа Grass
            if (textureCache.find("grassTexture") != textureCache.end())
                tile.shape.setTexture(&textureCache["grassTexture"]);
            else
                tile.shape.setFillColor(grassColor);
            if (y == riverY1 || y == riverY2) {
                tile.type = River;
                if (textureCache.find("riverTexture") != textureCache.end())
                    tile.shape.setTexture(&textureCache["riverTexture"]);
                else
                    tile.shape.setFillColor(riverColor);
            }
            if ((y < 1 && (x < 4 || x >= tilesX - 4)) ||
                (y >= tilesY - 1 && (x < 4 || x >= tilesX - 4))) {
                tile.type = Fence;
                tile.shape.setFillColor(fenceColor);
            }
            tile.shape.setTextureRect(sf::IntRect(0, 0, textureCache["riverTexture"].getSize().x, textureCache["riverTexture"].getSize().y));
            tile.shape.setOutlineThickness(1);
            tile.shape.setOutlineColor(sf::Color::Black);
            tiles.push_back(tile);
        }
    }


    for (auto [x, y] : bridgeCoords) {
        for (int dy = -1; dy <= 2; ++dy) {
            Tile bridgeTile;
            bridgeTile.shape.setSize(sf::Vector2f(tileSize - 1, tileSize - 1));
            bridgeTile.shape.setPosition(static_cast<float>(x * tileSize),
                static_cast<float>((y + dy) * tileSize));
            bridgeTile.type = Bridge;
            if (textureCache.find("bridgeTexture") != textureCache.end()) {
                bridgeTile.shape.setTexture(&textureCache["bridgeTexture"]);
            }
            else {
                bridgeTile.shape.setFillColor(bridgeColor);
            }

            bridgeTile.shape.setOutlineThickness(1);
            bridgeTile.shape.setOutlineColor(sf::Color::Black);

            bridges.push_back(bridgeTile);
        }
    }


    std::vector<Tower> towers;
    std::vector<Tower> friendlyTowers;

    auto createTower = [](sf::Vector2f size, sf::Vector2f position, sf::Color color) -> Tower {
        Tower tower;
        tower.shape.setSize(size);
        tower.shape.setFillColor(color);
        tower.shape.setPosition(position);
        tower.maxHealth = 1000;
        tower.health = tower.maxHealth;
        tower.healthBar.setSize({ size.x, 8 });
        tower.healthBar.setFillColor(sf::Color::Green);
        tower.healthBar.setPosition(position.x, position.y - 10);
        return tower;
        };

    towers.push_back(createTower({ tileSize * 4.f, tileSize * 4.f }, { tileSize * 7.f, tileSize * 1.f }, sf::Color(150, 0, 0)));
    towers.push_back(createTower({ tileSize * 3.f, tileSize * 3.f }, { tileSize * 2.f, tileSize * 5.f }, sf::Color(200, 50, 50)));
    towers.push_back(createTower({ tileSize * 3.f, tileSize * 3.f }, { tileSize * 13.f, tileSize * 5.f }, sf::Color(200, 50, 50)));

    friendlyTowers.push_back(createTower({ tileSize * 4.f, tileSize * 4.f }, { tileSize * 7.f, tileSize * (tilesY - 5.f) }, sf::Color(0, 0, 200)));
    friendlyTowers.push_back(createTower({ tileSize * 3.f, tileSize * 3.f }, { tileSize * 2.f, tileSize * (tilesY - 8.f) }, sf::Color(50, 50, 255)));
    friendlyTowers.push_back(createTower({ tileSize * 3.f, tileSize * 3.f }, { tileSize * 13.f, tileSize * (tilesY - 8.f) }, sf::Color(50, 50, 255)));

    std::vector<Unit> units;
    std::vector<Projectile> projectiles;
    std::vector<UnitProjectile> unitProjectiles;

    const float maxMana = 10.f;
    float playerMana = 0.f;
    const float manaRegenRate = 0.01f;

    const float enemyMaxMana = 10.f;
    float enemyMana = enemyMaxMana;
    const float enemyManaRegenRate = 0.01f;

    const float enemyAISummonInterval = 7.5f;
    sf::Clock enemyAIClock;

    sf::Vector2f manaBarPos(10, fieldHeight + 5);
    sf::Vector2f manaBarSize(windowWidth - 20, manaBarHeight - 10);
    sf::RectangleShape manaBarBackground(manaBarSize);
    manaBarBackground.setFillColor(sf::Color(50, 50, 50, 200));
    manaBarBackground.setPosition(manaBarPos);

    sf::RectangleShape manaBarFill;
    manaBarFill.setSize(sf::Vector2f(0, manaBarSize.y));
    manaBarFill.setFillColor(sf::Color::Blue);
    manaBarFill.setPosition(manaBarPos);

    int menuY = fieldHeight + manaBarHeight;

    auto posAllowed = [&](sf::Vector2f pos) -> bool {
        return isPositionAllowed(pos, tiles, bridges, towers, friendlyTowers);
        };

    bool enemyFrontTowerDestroyed = false;

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Texture crownRedTexture;
    if (!crownRedTexture.loadFromFile("crown_red.png")) {
        std::cerr << "Error crown_red.png" << std::endl;
    }
    sf::Texture crownBlueTexture;
    if (!crownBlueTexture.loadFromFile("crown_blue.png")) {
        std::cerr << "Error crown_blue.png" << std::endl;
    }

    sf::Text playerCrownText, enemyCrownText;
    playerCrownText.setFont(font);
    enemyCrownText.setFont(font);
    playerCrownText.setCharacterSize(20);
    enemyCrownText.setCharacterSize(20);
    playerCrownText.setFillColor(sf::Color::Blue);
    enemyCrownText.setFillColor(sf::Color::Red);
    playerCrownText.setOutlineColor(sf::Color::Black);
    enemyCrownText.setOutlineColor(sf::Color::Black);
    playerCrownText.setOutlineThickness(2);
    enemyCrownText.setOutlineThickness(2);
    playerCrownText.setStyle(sf::Text::Bold);
    enemyCrownText.setStyle(sf::Text::Bold);
    enemyCrownText.setString(std::to_string(enemyCrowns));

    sf::RectangleShape playerCrownBackground, enemyCrownBackground;
    playerCrownBackground.setFillColor(sf::Color(255, 255, 255, 180));
    playerCrownBackground.setOutlineThickness(2);
    playerCrownBackground.setOutlineColor(sf::Color::Black);
    playerCrownBackground.setSize(sf::Vector2f(33, 60));

    enemyCrownBackground.setFillColor(sf::Color(255, 255, 255, 180));
    enemyCrownBackground.setOutlineThickness(2);
    enemyCrownBackground.setOutlineColor(sf::Color::Black);
    enemyCrownBackground.setSize(sf::Vector2f(33, 60));

    sf::Sprite crownBlueSprite, crownRedSprite;
    crownBlueSprite.setTexture(crownBlueTexture);
    crownRedSprite.setTexture(crownRedTexture);
    crownBlueSprite.setScale(0.1f, 0.1f);
    crownRedSprite.setScale(0.3f, 0.3f);

    sf::RectangleShape menuBackground;
    menuBackground.setSize(sf::Vector2f(windowWidth, menuHeight));
    menuBackground.setFillColor(sf::Color(50, 50, 50, 200));
    menuBackground.setPosition(0, menuY);

    const float scalingFactor = static_cast<float>(targetHeight) / baseWindowHeight;

    const int nextCardWidth = 38;
    const int nextCardHeight = 57;
    sf::RectangleShape nextCard(sf::Vector2f(nextCardWidth, nextCardHeight));
    nextCard.setFillColor(sf::Color::White);
    nextCard.setOutlineColor(sf::Color::Black);
    nextCard.setOutlineThickness(2);
    float nextPosX = baseNextCardXOffset * scalingFactor;
    float nextPosY = menuY + (menuHeight - nextCardHeight) / 1.4f;
    nextCard.setPosition(nextPosX, nextPosY);

    sf::Text nextLabel;
    nextLabel.setFont(font);
    nextLabel.setString("NEXT");
    nextLabel.setCharacterSize(18);
    nextLabel.setFillColor(sf::Color::White);
    sf::FloatRect textRect = nextLabel.getLocalBounds();
    nextLabel.setOrigin(textRect.left + textRect.width / 2.f,
        textRect.top + textRect.height / 2.f);
    nextLabel.setPosition(
        nextCard.getPosition().x + nextCard.getSize().x / 2.f,
        nextCard.getPosition().y - 15
    );

    const int cardWidth = 60;
    const int cardHeight = 80;
    const int deckCardSpacing = 20;
    float deckStartX = nextPosX + nextCardWidth + deckCardSpacing + 22;
    std::vector<sf::RectangleShape> deckCards;
    for (int i = 0; i < numDeckCards; i++) {
        sf::RectangleShape card(sf::Vector2f(cardWidth, cardHeight));
        card.setFillColor(sf::Color::White);
        card.setOutlineColor(sf::Color::Black);
        card.setOutlineThickness(2);
        float posX = deckStartX + i * (cardWidth + deckCardSpacing);
        float posY = menuY + (menuHeight - cardHeight) / 2.f;
        card.setPosition(posX, posY);
        deckCards.push_back(card);
    }

    sf::Text nextCardName;
    nextCardName.setFont(font);
    nextCardName.setCharacterSize(14);
    nextCardName.setFillColor(sf::Color::Black);
    int selectedCardIndex = -1;

    std::vector<sf::Text> deckCardTexts;
    for (int i = 0; i < numDeckCards; i++) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        deckCardTexts.push_back(text);
    }

    sf::RectangleShape uiBackground;
    uiBackground.setSize(sf::Vector2f(windowWidth, manaBarHeight + menuHeight));
    uiBackground.setFillColor(sf::Color::Black);
    uiBackground.setPosition(698, fieldHeight);


    auto updateDeckDisplay = [&]() {
        for (int i = 0; i < numDeckCards; i++) {
            int idx = deckIndices[i];
            if (!availableUnits[idx].splashArtPath.empty()) {
                if (textureCache.find(availableUnits[idx].splashArtPath) == textureCache.end()) {
                    sf::Texture texture;
                    if (!texture.loadFromFile(availableUnits[idx].splashArtPath)) {
                        std::cerr << "Error "
                            << availableUnits[idx].splashArtPath << std::endl;
                        deckCards[i].setTexture(nullptr);
                        deckCards[i].setFillColor(availableUnits[idx].color);
                    }
                    else {
                        texture.setSmooth(true);
                        textureCache[availableUnits[idx].splashArtPath] = texture;
                        deckCards[i].setTexture(&textureCache[availableUnits[idx].splashArtPath]);
                    }
                }
                else {
                    deckCards[i].setTexture(&textureCache[availableUnits[idx].splashArtPath]);
                }
                deckCardTexts[i].setString("");
            }
            else {
                deckCards[i].setTexture(nullptr);
                deckCards[i].setFillColor(availableUnits[idx].color);
                deckCardTexts[i].setString(availableUnits[idx].name);
                sf::FloatRect textRect = deckCardTexts[i].getLocalBounds();
                deckCardTexts[i].setOrigin(textRect.left + textRect.width / 2.f,
                    textRect.top + textRect.height / 2.f);
                deckCardTexts[i].setPosition(
                    deckCards[i].getPosition().x + deckCards[i].getSize().x / 2.f,
                    deckCards[i].getPosition().y + deckCards[i].getSize().y / 2.f
                );
            }
            // Подсвечиваем выбранный слот
            if (i == selectedCardIndex) {
                deckCards[i].setOutlineThickness(4);
                deckCards[i].setOutlineColor(sf::Color::Yellow);
            }
            else {
                deckCards[i].setOutlineThickness(2);
                deckCards[i].setOutlineColor(sf::Color::Black);
            }
        }
        // Обновляем карточку "NEXT" аналогичным образом:
        if (!availableUnits[nextCardIndex].splashArtPath.empty()) {
            if (textureCache.find(availableUnits[nextCardIndex].splashArtPath) == textureCache.end()) {
                sf::Texture texture;
                if (!texture.loadFromFile(availableUnits[nextCardIndex].splashArtPath)) {
                    std::cerr << "Error of loading: "
                        << availableUnits[nextCardIndex].splashArtPath << std::endl;
                    nextCard.setTexture(nullptr);
                    nextCard.setFillColor(availableUnits[nextCardIndex].color);
                }
                else {
                    texture.setSmooth(true);
                    textureCache[availableUnits[nextCardIndex].splashArtPath] = texture;
                    nextCard.setTexture(&textureCache[availableUnits[nextCardIndex].splashArtPath]);
                }
            }
            else {
                nextCard.setTexture(&textureCache[availableUnits[nextCardIndex].splashArtPath]);
            }
            nextCardName.setString("");
        }
        else {
            nextCard.setTexture(nullptr);
            nextCard.setFillColor(availableUnits[nextCardIndex].color);
            nextCardName.setString(availableUnits[nextCardIndex].name);
            sf::FloatRect nextTextRect = nextCardName.getLocalBounds();
            nextCardName.setOrigin(nextTextRect.left + nextTextRect.width / 2.f,
                nextTextRect.top + nextTextRect.height / 2.f);
            nextCardName.setPosition(
                nextCard.getPosition().x + nextCard.getSize().x / 2.f,
                nextCard.getPosition().y + nextCard.getSize().y / 2.f
            );

        }
        };



    updateDeckDisplay();

    bool leftEnemyTowerAlive = true;
    bool rightEnemyTowerAlive = true;

    const int playerSummonY = (riverY2 + 1) * tileSize;

    sf::Clock deltaClock;

    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f clickPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gameView);
                if (clickPos.y >= menuY) {
                    bool cardClicked = false;
                    int clickedCardIdx = -1;
                    for (int i = 0; i < numDeckCards; i++) {
                        if (deckCards[i].getGlobalBounds().contains(clickPos)) {
                            cardClicked = true;
                            clickedCardIdx = i;
                            break;
                        }
                    }
                    if (cardClicked) {
                        selectedCardIndex = clickedCardIdx;
                        updateDeckDisplay();
                    }
                    continue;
                }
                if (selectedCardIndex == -1)
                    continue;
                int tx = static_cast<int>(clickPos.x) / tileSize;
                int ty = static_cast<int>(clickPos.y) / tileSize;
                if (tx < 0 || tx >= tilesX || ty < 0 || ty >= tilesY)
                    continue;
                sf::Vector2f cellCenter = getTileCenter(tx, ty);
                TileType clickedTileType = tiles[ty * tilesX + tx].type;
                bool validPlacement = false, enemySummon = false, friendlySummon = false;
                if (clickedTileType == River || clickedTileType == Fence)
                    continue;
                const float friendlySummonThreshold = static_cast<float>((riverY2 + 1) * tileSize);
                if (cellCenter.y >= friendlySummonThreshold) {
                    validPlacement = true;
                    friendlySummon = true;
                }
                else {
                    int enemyZoneY = riverY1 - 4;
                    sf::IntRect leftEnemySummonZone(0, enemyZoneY * tileSize, 9 * tileSize, 4 * tileSize);
                    sf::IntRect rightEnemySummonZone(windowWidth - (9 * tileSize), enemyZoneY * tileSize, 9 * tileSize, 4 * tileSize);
                    if ((!leftEnemyTowerAlive && leftEnemySummonZone.contains(static_cast<int>(cellCenter.x), static_cast<int>(cellCenter.y))) ||
                        (!rightEnemyTowerAlive && rightEnemySummonZone.contains(static_cast<int>(cellCenter.x), static_cast<int>(cellCenter.y)))) {
                        validPlacement = true;
                        enemySummon = true;
                    }
                }
                for (auto& tower : towers)
                    if (tower.shape.getGlobalBounds().contains(cellCenter))
                        validPlacement = false;
                for (auto& tower : friendlyTowers)
                    if (tower.shape.getGlobalBounds().contains(cellCenter))
                        validPlacement = false;
                if (!validPlacement)
                    continue;
                int unitIndex = deckIndices[selectedCardIndex];
                float cost = availableUnits[unitIndex].manaCost;
                if (playerMana < cost)
                    continue;
                playerMana -= cost;
                Unit unit = createUnitFromStats(availableUnits[unitIndex], cellCenter);
                if (enemySummon) {
                    unit.crossedRiver = true;
                    float minDist = std::numeric_limits<float>::max();
                    sf::Vector2f closestTower;
                    for (auto& tower : towers) {
                        sf::FloatRect tBounds = tower.shape.getGlobalBounds();
                        sf::Vector2f towerCenter(tBounds.left + tBounds.width / 2.f, tBounds.top + tBounds.height / 2.f);
                        float d = distance(towerCenter, unit.shape.getPosition());
                        if (d < minDist) {
                            minDist = d;
                            closestTower = towerCenter;
                        }
                    }
                    unit.target = closestTower;
                }
                else if (friendlySummon) {
                    unit.crossedRiver = false;
                    sf::Vector2f closestBridge = getTileCenter(bridgeCoords[0].x, riverY1 + 1);
                    float minDist = distance(closestBridge, cellCenter);
                    for (auto [bx, by] : bridgeCoords) {
                        sf::Vector2f bridgeCenter = getTileCenter(bx, riverY1 + 1);
                        float d = distance(bridgeCenter, cellCenter);
                        if (d < minDist) {
                            minDist = d;
                            closestBridge = bridgeCenter;
                        }
                    }
                    unit.target = closestBridge;
                }
                units.push_back(unit);
                deckIndices.erase(deckIndices.begin() + selectedCardIndex);
                deckIndices.push_back(nextCardIndex);
                {
                    std::set<int> temp;
                    for (auto idx : deckIndices)
                        temp.insert(idx);
                    do {
                        nextCardIndex = rand() % availableUnits.size();
                    } while (temp.find(nextCardIndex) != temp.end());
                }
                selectedCardIndex = -1;
                updateDeckDisplay();
                continue;
            }
        }

        if (playerMana < maxMana) {
            playerMana += manaRegenRate;
            if (playerMana > maxMana)
                playerMana = maxMana;
        }
        float segmentWidth = manaBarBackground.getSize().x / maxMana;
        float manaFillWidth = (playerMana / maxMana) * manaBarBackground.getSize().x;
        manaBarFill.setSize(sf::Vector2f(manaFillWidth, manaBarBackground.getSize().y));

        if (enemyMana < enemyMaxMana) {
            enemyMana += enemyManaRegenRate;
            if (enemyMana > enemyMaxMana)
                enemyMana = enemyMaxMana;
        }

        if (enemyAIClock.getElapsedTime().asSeconds() >= enemyAISummonInterval && !gameOver) {
            std::vector<int> affordableIndices;
            for (int i = 0; i < availableUnits.size(); i++) {
                if (availableUnits[i].manaCost <= enemyMana)
                    affordableIndices.push_back(i);
            }
            if (!affordableIndices.empty()) {
                int randIndex = affordableIndices[rand() % affordableIndices.size()];
                enemyMana -= availableUnits[randIndex].manaCost;
                int enemySpawnXMin = tileSize * 1;
                int enemySpawnXMax = tileSize * 17;
                int enemySpawnYMin = tileSize * 8;
                int enemySpawnYMax = (riverY1 * tileSize) - tileSize;

                float spawnX = static_cast<float>(enemySpawnXMin + rand() % (enemySpawnXMax - enemySpawnXMin));
                float spawnY = static_cast<float>(enemySpawnYMin + rand() % (enemySpawnYMax - enemySpawnYMin));
                sf::Vector2f spawnPos(spawnX, spawnY);
                Unit enemyUnit = createUnitFromStats(availableUnits[randIndex], spawnPos);
                enemyUnit.isEnemy = true;
                enemyUnit.crossedRiver = false;
                enemyUnit.shape.setOutlineThickness(2.f);
                enemyUnit.shape.setOutlineColor(sf::Color::Red);
                sf::Vector2f closestBridge = getTileCenter(bridgeCoords[0].x, riverY1);
                float minDist = distance(spawnPos, closestBridge);
                for (auto const& bc : bridgeCoords) {
                    sf::Vector2f bridgeCenter = getTileCenter(bc.x, riverY1);
                    float d = distance(spawnPos, bridgeCenter);
                    if (d < minDist) { minDist = d; closestBridge = bridgeCenter; }
                }
                enemyUnit.target = closestBridge;
                units.push_back(enemyUnit);
            }
            enemyAIClock.restart();
        }

        for (auto& unit : units) {
            if (unit.stats.speed == 0.f) {
                unit.health -= 20.f * dt;
            }
            if (!unit.isEnemy) {
                unit.shape.setOutlineThickness(2.f);
                unit.shape.setOutlineColor(sf::Color::Blue);
                if (unit.enemyTarget != nullptr) {
                    if (unit.enemyTarget->health <= 0 ||
                        distance(unit.shape.getPosition(), unit.enemyTarget->shape.getPosition()) > unit.stats.agroRange ||
                        unit.enemyTarget == &unit ||
                        !unit.enemyTarget->isEnemy)
                    {
                        unit.enemyTarget = nullptr;
                    }
                }
                if (unit.enemyTarget == nullptr) {
                    for (auto& potentialEnemy : units) {
                        if (potentialEnemy.isEnemy && potentialEnemy.health > 0 && &potentialEnemy != &unit) {
                            if (distance(unit.shape.getPosition(), potentialEnemy.shape.getPosition()) <= unit.stats.agroRange) {
                                unit.enemyTarget = &potentialEnemy;
                                break;
                            }
                        }
                    }
                }
                if (unit.enemyTarget != nullptr)
                    unit.target = unit.enemyTarget->shape.getPosition();
            }

            else {
                if (unit.enemyTarget != nullptr) {
                    if (unit.enemyTarget != nullptr) {
                        if (unit.enemyTarget->health <= 0 ||
                            distance(unit.shape.getPosition(), unit.enemyTarget->shape.getPosition()) > unit.stats.agroRange ||
                            unit.enemyTarget == &unit ||
                            unit.enemyTarget->isEnemy)
                        {
                            unit.enemyTarget = nullptr;
                        }
                    }

                    {
                        unit.enemyTarget = nullptr;
                    }
                }
                if (unit.enemyTarget == nullptr) {
                    for (auto& potentialTarget : units) {
                        if (!potentialTarget.isEnemy && potentialTarget.health > 0 && &potentialTarget != &unit) {
                            if (distance(unit.shape.getPosition(), potentialTarget.shape.getPosition()) <= unit.stats.agroRange) {
                                unit.enemyTarget = &potentialTarget;
                                break;
                            }
                        }
                    }
                }
                if (unit.enemyTarget != nullptr)
                    unit.target = unit.enemyTarget->shape.getPosition();
            }
        }

        for (auto& unit : units) {
            unit.attackCooldown = std::max(0.f, unit.attackCooldown - dt);
        }
        for (auto& tower : towers) {
            tower.attackCooldown = std::max(0.f, tower.attackCooldown - dt);
        }
        for (auto& tower : friendlyTowers) {
            tower.attackCooldown = std::max(0.f, tower.attackCooldown - dt);
        }

        auto attemptMove = [&](Unit& unit, const sf::Vector2f& desiredDir) {
            float moveDist = unit.stats.speed * dt * speedScale;
            sf::Vector2f pos = unit.shape.getPosition();
            sf::Vector2f cand = pos + desiredDir * moveDist;
            if (posAllowed(cand))
                unit.shape.move(desiredDir * moveDist);
            else {
                sf::Vector2f alt1 = rotateVector(desiredDir, 3.14159f / 4.f);
                cand = pos + alt1 * moveDist;
                if (posAllowed(cand))
                    unit.shape.move(alt1 * moveDist);
                else {
                    sf::Vector2f alt2 = rotateVector(desiredDir, -3.14159f / 4.f);
                    cand = pos + alt2 * moveDist;
                    if (posAllowed(cand))
                        unit.shape.move(alt2 * moveDist);
                }
            }
            };

        for (auto& unit : units) {

            bool targetInAttackRange = false;
            if (unit.enemyTarget != nullptr) {
                float dTarget = distance(unit.shape.getPosition(), unit.enemyTarget->shape.getPosition());
                if (dTarget <= unit.stats.attackRange)
                    targetInAttackRange = true;
            }

            bool towerInAttackRange = false;
            Tower* towerTarget = nullptr;
            if (unit.enemyTarget == nullptr) {
                float minTowerDist = std::numeric_limits<float>::max();
                if (!unit.isEnemy) {
                    for (auto& t : towers) {
                        sf::FloatRect tb = t.shape.getGlobalBounds();
                        sf::Vector2f candidate = getTowerTargetPoint(tb, unit.shape.getPosition(), unit.stats.attackRange);
                        float dTower = distance(unit.shape.getPosition(), candidate);
                        if (dTower < minTowerDist) {
                            minTowerDist = dTower;
                            towerTarget = &t;
                        }
                    }
                }
                else {
                    for (auto& t : friendlyTowers) {
                        sf::FloatRect tb = t.shape.getGlobalBounds();
                        sf::Vector2f candidate = getTowerTargetPoint(tb, unit.shape.getPosition(), unit.stats.attackRange);
                        float dTower = distance(unit.shape.getPosition(), candidate);
                        if (dTower < minTowerDist) {
                            minTowerDist = dTower;
                            towerTarget = &t;
                        }
                    }
                }
                if (towerTarget && minTowerDist <= unit.stats.attackRange) {
                    towerInAttackRange = true;
                }
            }

            if ((targetInAttackRange || towerInAttackRange) && unit.attackCooldown <= 0) {
                if (unit.stats.attackRange > 120.f) {
                    Projectile p;
                    p.shape.setRadius(3);
                    p.shape.setFillColor(unit.stats.color);
                    p.shape.setOrigin(3, 3);
                    p.shape.setPosition(unit.shape.getPosition());
                    p.damage = unit.stats.damage;
                    sf::Vector2f attackTarget;
                    if (targetInAttackRange) {
                        sf::FloatRect tBound = unit.enemyTarget->shape.getGlobalBounds();
                        attackTarget = sf::Vector2f(tBound.left + tBound.width / 2.f,
                            tBound.top + tBound.height / 2.f);
                        p.target = unit.enemyTarget;
                        p.staticTarget = sf::Vector2f(0, 0);
                    }
                    else if (towerInAttackRange && towerTarget) {
                        sf::FloatRect tBound = towerTarget->shape.getGlobalBounds();
                        attackTarget = sf::Vector2f(tBound.left + tBound.width / 2.f,
                            tBound.top + tBound.height / 2.f);
                        p.target = nullptr;
                        p.staticTarget = attackTarget;
                    }
                    sf::Vector2f attackDir = attackTarget - unit.shape.getPosition();
                    float len = std::sqrt(attackDir.x * attackDir.x + attackDir.y * attackDir.y);
                    if (len != 0)
                        attackDir /= len;
                    p.velocity = attackDir * 4.f;
                    projectiles.push_back(p);
                    unit.attackCooldown = unit.stats.attackCooldown;
                }
                else {
                    if (unit.enemyTarget != nullptr && targetInAttackRange) {
                        unit.enemyTarget->health -= unit.stats.damage;
                    }
                    else if (towerInAttackRange && towerTarget) {
                        towerTarget->health -= unit.stats.damage;
                    }
                    unit.attackCooldown = unit.stats.attackCooldown;
                }
            }
            else {
                if (targetInAttackRange || towerInAttackRange) {
                }
                else {
                    float d = distance(unit.shape.getPosition(), unit.target);
                    if (d > 10.f) {
                        sf::Vector2f pos = unit.shape.getPosition();
                        sf::Vector2f desiredDir = unit.target - pos;
                        float len = std::sqrt(desiredDir.x * desiredDir.x + desiredDir.y * desiredDir.y);
                        if (len != 0) {
                            desiredDir /= len;
                            const int maxSteps = 6;
                            bool moved = false;
                            for (int i = 0; i <= maxSteps && !moved; i++) {
                                float angle = i * (3.14159f / 12.0f);
                                sf::Vector2f cand = pos + rotateVector(desiredDir, angle) * unit.stats.speed * dt * speedScale;
                                if (posAllowed(cand)) {
                                    unit.shape.move(rotateVector(desiredDir, angle) * unit.stats.speed * dt * speedScale);
                                    moved = true;
                                }
                                if (!moved && i > 0) {
                                    cand = pos + rotateVector(desiredDir, -angle) * unit.stats.speed * dt * speedScale;
                                    if (posAllowed(cand)) {
                                        unit.shape.move(rotateVector(desiredDir, -angle) * unit.stats.speed * dt * speedScale);
                                        moved = true;
                                    }
                                }
                            }
                        }
                    }
                    else {
                        float minDist = std::numeric_limits<float>::max();
                        sf::Vector2f newTarget = unit.target;
                        const auto& towersToUse = (unit.isEnemy ? friendlyTowers : towers);
                        for (auto& tower : towersToUse) {
                            sf::FloatRect tb = tower.shape.getGlobalBounds();
                            sf::Vector2f candidate = getTowerTargetPoint(tb, unit.shape.getPosition(), unit.stats.attackRange);
                            sf::Vector2f dir = candidate - unit.shape.getPosition();
                            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                            if (len != 0) {
                                dir /= len;
                                candidate -= dir * 2.f;
                            }
                            float dCandidate = distance(unit.shape.getPosition(), candidate);
                            if (dCandidate < minDist) {
                                minDist = dCandidate;
                                newTarget = candidate;
                            }
                        }
                        unit.target = newTarget;
                    }
                }
            }

            unit.healthBar.setPosition(unit.shape.getPosition().x, unit.shape.getPosition().y - 20);
            unit.healthBar.setSize({ 20 * (unit.health / unit.maxHealth), 5 });
        }

        for (auto& tower : towers) {
            float attackRange = 150.f;
            Unit* closest = nullptr;
            float minDist = attackRange;
            for (auto& unit : units) {
                if (!unit.isEnemy) {
                    sf::FloatRect uBounds = unit.shape.getGlobalBounds();
                    sf::Vector2f uCenter(uBounds.left + uBounds.width / 2.f,
                        uBounds.top + uBounds.height / 2.f);
                    sf::FloatRect tBounds = tower.shape.getGlobalBounds();
                    sf::Vector2f tCenter(tBounds.left + tBounds.width / 2.f,
                        tBounds.top + tBounds.height / 2.f);
                    float d = distance(tCenter, uCenter);
                    if (d < minDist) {
                        minDist = d;
                        closest = &unit;
                    }
                }
            }
            if (closest && tower.attackCooldown <= 0) {
                Projectile p;
                p.shape.setRadius(5);
                p.shape.setFillColor(sf::Color::Yellow);
                sf::FloatRect tBounds = tower.shape.getGlobalBounds();
                sf::Vector2f tCenter(tBounds.left + tBounds.width / 2.f,
                    tBounds.top + tBounds.height / 2.f);
                p.shape.setPosition(tCenter);
                p.target = closest;
                p.damage = 30;
                sf::FloatRect uBounds = closest->shape.getGlobalBounds();
                sf::Vector2f uCenter(uBounds.left + uBounds.width / 2.f,
                    uBounds.top + uBounds.height / 2.f);
                sf::Vector2f dir = uCenter - tCenter;
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len != 0) { dir /= len; }
                p.velocity = dir * 3.f;
                projectiles.push_back(p);
                tower.attackCooldown = 5;
            }
            sf::Vector2f size = tower.shape.getSize();
            tower.healthBar.setSize({ size.x * (tower.health / tower.maxHealth), 8 });
            tower.healthBar.setPosition(tower.shape.getPosition().x, tower.shape.getPosition().y - 10);
        }

        for (auto& tower : friendlyTowers) {
            float attackRange = 150.f;
            Unit* closest = nullptr;
            float minDist = attackRange;
            for (auto& unit : units) {
                if (unit.isEnemy) {
                    sf::FloatRect uBounds = unit.shape.getGlobalBounds();
                    sf::Vector2f uCenter(uBounds.left + uBounds.width / 2.f,
                        uBounds.top + uBounds.height / 2.f);
                    sf::FloatRect tBounds = tower.shape.getGlobalBounds();
                    sf::Vector2f tCenter(tBounds.left + tBounds.width / 2.f,
                        tBounds.top + tBounds.height / 2.f);
                    float d = distance(tCenter, uCenter);
                    if (d < minDist) {
                        minDist = d;
                        closest = &unit;
                    }
                }
            }
            if (closest && tower.attackCooldown <= 0) {
                Projectile p;
                p.shape.setRadius(5);
                p.shape.setFillColor(sf::Color::Yellow);
                sf::FloatRect tBounds = tower.shape.getGlobalBounds();
                sf::Vector2f tCenter(tBounds.left + tBounds.width / 2.f,
                    tBounds.top + tBounds.height / 2.f);
                p.shape.setPosition(tCenter);
                p.target = closest;
                p.damage = 30;
                sf::FloatRect uBounds = closest->shape.getGlobalBounds();
                sf::Vector2f uCenter(uBounds.left + uBounds.width / 2.f,
                    uBounds.top + uBounds.height / 2.f);
                sf::Vector2f dir = uCenter - tCenter;
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len != 0) { dir /= len; }
                p.velocity = dir * 3.f;
                projectiles.push_back(p);
                tower.attackCooldown = 5;
            }
            sf::Vector2f size = tower.shape.getSize();
            tower.healthBar.setSize({ size.x * (tower.health / tower.maxHealth), 8 });
            tower.healthBar.setPosition(tower.shape.getPosition().x, tower.shape.getPosition().y - 10);
        }

        for (auto it = projectiles.begin(); it != projectiles.end(); ) {
            if (it->target == nullptr) {
                it = projectiles.erase(it);
                continue;
            }

            if (it->target->health <= 0) {
                it = projectiles.erase(it);
                continue;
            }

            sf::FloatRect targetBounds = it->target->shape.getGlobalBounds();
            sf::Vector2f targetCenter(targetBounds.left + targetBounds.width / 2.f,
                targetBounds.top + targetBounds.height / 2.f);

            sf::Vector2f dir = targetCenter - it->shape.getPosition();
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (len != 0)
                dir /= len;

            const float projectileSpeed = 4.f;
            it->velocity = dir * projectileSpeed;

            const float hitThreshold = 5.f;
            if (len < hitThreshold) {
                it->target->health -= it->damage;
                it = projectiles.erase(it);
                continue;
            }

            it->shape.move(it->velocity);
            ++it;
        }




        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& p) {
            return !p.target ||
                p.shape.getPosition().x < 0 || p.shape.getPosition().y < 0 ||
                p.shape.getPosition().x > windowWidth || p.shape.getPosition().y > windowHeight;
            }), projectiles.end());

        for (auto& up : unitProjectiles) {
            if (up.target) {
                sf::FloatRect tBounds = up.target->shape.getGlobalBounds();
                sf::Vector2f tCenter(tBounds.left + tBounds.width / 2.f,
                    tBounds.top + tBounds.height / 2.f);
                if (distance(up.shape.getPosition(), tCenter) < 10.f) {
                    up.target->health -= up.damage;
                    up.target = nullptr;
                }
                else {
                    up.shape.move(up.velocity);
                }
            }
        }
        unitProjectiles.erase(std::remove_if(unitProjectiles.begin(), unitProjectiles.end(), [&](const UnitProjectile& up) {
            return (!up.target) || (up.shape.getPosition().x < 0 || up.shape.getPosition().y < 0 ||
                up.shape.getPosition().x > windowWidth || up.shape.getPosition().y > windowHeight);
            }), unitProjectiles.end());

        units.erase(std::remove_if(units.begin(), units.end(), [](const Unit& u) {
            return u.health <= 0;
            }), units.end());

        for (auto it = towers.begin(); it != towers.end();) {
            if (it->health <= 0) {
                sf::Vector2f pos = it->shape.getPosition();
                if (std::abs(pos.x - tileSize * 7.f) < 1.f && std::abs(pos.y - tileSize * 1.f) < 1.f) {
                    crowns = 3;
                    gameOver = true;
                    towers.clear();
                    break;
                }
                else {
                    crowns += 1;
                    it = towers.erase(it);
                    continue;
                }
            }
            else {
                ++it;
            }
        }

        for (auto it = friendlyTowers.begin(); it != friendlyTowers.end();) {
            if (it->health <= 0) {
                sf::Vector2f pos = it->shape.getPosition();
                if (std::abs(pos.x - tileSize * 7.f) < 1.f && std::abs(pos.y - tileSize * (tilesY - 5.f)) < 1.f) {
                    enemyCrowns = 3;
                    gameOver = true;
                    friendlyTowers.clear();
                    break;
                }
                else {
                    enemyCrowns += 1;
                    it = friendlyTowers.erase(it);
                    continue;
                }
            }
            else {
                ++it;
            }
        }


        towers.erase(std::remove_if(towers.begin(), towers.end(), [](const Tower& t) {
            return t.health <= 0;
            }), towers.end());
        friendlyTowers.erase(std::remove_if(friendlyTowers.begin(), friendlyTowers.end(), [](const Tower& t) {
            return t.health <= 0;
            }), friendlyTowers.end());

        window.clear();

        window.setView(window.getDefaultView());
        window.draw(backgroundSprite);
        window.draw(uiBackground);
        window.setView(gameView);

        for (auto& tile : tiles)
            window.draw(tile.shape);
        for (auto& bridge : bridges)
            window.draw(bridge.shape);
        for (auto& tower : towers) {
            window.draw(tower.shape);
            window.draw(tower.healthBar);
            if (!enemyFrontTowerDestroyed && tower.health <= 0) {
                sf::Vector2f pos = tower.shape.getPosition();
                if (pos.y < riverY1 * tileSize)
                    enemyFrontTowerDestroyed = true;
            }
        }
        for (auto& ft : friendlyTowers) {
            window.draw(ft.shape);
            window.draw(ft.healthBar);
        }
        for (auto& unit : units) {
            window.draw(unit.shape);
            window.draw(unit.healthBar);
        }
        for (auto& p : projectiles)
            window.draw(p.shape);
        for (auto& up : unitProjectiles)
            window.draw(up.shape);
        window.draw(manaBarBackground);
        window.draw(manaBarFill);
        for (int i = 1; i < static_cast<int>(maxMana); ++i) {
            sf::RectangleShape divider;
            divider.setSize(sf::Vector2f(2, manaBarBackground.getSize().y));
            divider.setFillColor(sf::Color::White);
            divider.setPosition(manaBarBackground.getPosition().x + i * segmentWidth, manaBarBackground.getPosition().y);
            window.draw(divider);
        }

        float crownX = windowWidth - 50.f;
        playerCrownText.setString(std::to_string(crowns));
        playerCrownText.setPosition(crownX - 4.f, (riverY2 + 2.8f) * tileSize);
        crownBlueSprite.setPosition(crownX - 37.f, (riverY2 + 4.0f) * tileSize - 5.f);
        enemyCrownText.setPosition(crownX - 4.f, (riverY1 - 2.0f) * tileSize);
        crownRedSprite.setPosition(crownX - 37.f, (riverY1 - 4.2f) * tileSize - 5.f);
        playerCrownBackground.setPosition(crownX - 14.f, (riverY2 + 2.6f) * tileSize);
        enemyCrownBackground.setPosition(crownX - 14.f, (riverY1 - 3.0f) * tileSize - 5.f);

        window.draw(playerCrownBackground);
        window.draw(enemyCrownBackground);
        window.draw(menuBackground);
        window.draw(nextCard);
        window.draw(nextLabel);
        window.draw(nextCardName);
        for (int i = 0; i < numDeckCards; i++) {
            window.draw(deckCards[i]);
            window.draw(deckCardTexts[i]);
        }
        window.draw(crownBlueSprite);
        window.draw(playerCrownText);
        window.draw(crownRedSprite);
        window.draw(enemyCrownText);

        window.display();

        if (gameOver)
            continue;
    }

    return;
}