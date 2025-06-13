#include <iostream>
#include <vector>

#include "MenuButton.h"
#include "MainMenu.h"
#include "UserProfile.h"
#include "ManagerProfile.h"
#include "WindowDeck.h"
#include "Setting.h"
#include "BattleResultWindow.h"
#include "WindowShop.h"
#include "Game_k.h"


using namespace std;

void showMainMenu(sf::RenderWindow& window) {

    //Background

	sf::Texture textureBackground;
	textureBackground.loadFromFile("assets/images/Wallpaper Clash Royale PC.jpeg");
    
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0.f, 0.f);
    spriteBackground.setScale(0.2f, 0.2f);

    sf::Vector2u textureSize = textureBackground.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaletX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaletY = static_cast<float>(windowSize.y) / textureSize.y;
    spriteBackground.setScale(scaletX, scaletY);

    //Mini Arena

    sf::Texture miniArena;
    miniArena.loadFromFile("assets/images/Training_Camp.png");


    sf::Sprite spriteMiniArena;
    spriteMiniArena.setTexture(miniArena);
    spriteMiniArena.setScale(0.29f, 0.29f);
    
    sf::Vector2u spriteSizeMiniArena = miniArena.getSize();
    spriteMiniArena.setOrigin(spriteSizeMiniArena.x / 2.f, spriteSizeMiniArena.y / 2.f);
    spriteMiniArena.setPosition(windowSize.x / 2.f , windowSize.y / 2.f);

    //Left & Right Sprites characters

    sf::Texture texturLeftCharacters;
    texturLeftCharacters.loadFromFile("assets/images/1_l.png");

    sf::Sprite spriteLeftCharacters;
    spriteLeftCharacters.setTexture(texturLeftCharacters);
    spriteLeftCharacters.setScale(1.f, 1.f);

    sf::Texture texturRightCharacters;
    texturRightCharacters.loadFromFile("assets/images/2_r.png");

    sf::Sprite spriteRightCharacters;
    spriteRightCharacters.setTexture(texturRightCharacters);
    spriteRightCharacters.setScale(1.f, 1.f);

    sf::Vector2u spriteSizeCharacters = texturLeftCharacters.getSize();
    spriteLeftCharacters.setOrigin(spriteSizeCharacters.x / 2.f, spriteSizeCharacters.y / 2.f);
    spriteLeftCharacters.setPosition(170.f, windowSize.y - 240.f);

    spriteRightCharacters.setOrigin(spriteSizeCharacters.x / 2.f, spriteSizeCharacters.y / 2.f);
    spriteRightCharacters.setPosition(windowSize.x - 170.f, windowSize.y - 240.f);

    //Battle

    sf::Texture btnBattle;
    btnBattle.loadFromFile("assets/images/btnBattle.png");

    sf::Sprite spriteBtnBattle;
    spriteBtnBattle.setTexture(btnBattle);
    spriteBtnBattle.setScale(0.29f, 0.29f);
    spriteBtnBattle.setOrigin(btnBattle.getSize().x / 2.f, spriteMiniArena.getPosition().y + (spriteSizeMiniArena.y * 0.29f) / 2.f + 50.f);
    spriteBtnBattle.setPosition(windowSize.x / 2.f, windowSize.y - 100.f);



    //Bottom Buttons
    sf::Texture textureButton1, textureButton2, textureButton3;
    textureButton1.loadFromFile("assets/icons/Cards.png");
    textureButton2.loadFromFile("assets/icons/battle1.png");
    textureButton3.loadFromFile("assets/icons/ShopIcon.png");

    vector<MenuButton> buttons;
    buttons.emplace_back(&textureButton1, sf::Vector2f(820.f, 950.f), sf::Vector2f(80.f, 80.f));
    buttons.emplace_back(&textureButton2, sf::Vector2f(930.f, 950.f), sf::Vector2f(80.f, 80.f));
    buttons.emplace_back(&textureButton3, sf::Vector2f(1040.f, 950.f), sf::Vector2f(80.f, 80.f));

    //Bottom Line for bottom buttons

    sf::Texture bottomLine;
    bottomLine.loadFromFile("assets/images/bottomLine.png");

    sf::Sprite spriteBottomLine;
    spriteBottomLine.setTexture(bottomLine);
    spriteBottomLine.setScale(0.3f, 0.5f);
    spriteBottomLine.setOrigin(bottomLine.getSize().x / 2.f, bottomLine.getSize().y / 2.f);
    spriteBottomLine.setPosition(windowSize.x / 2.f, 990.f);


    //Politechnika baner

    sf::Texture texturePolitechnika;
    if (!texturePolitechnika.loadFromFile("assets/icons/politechnikaBaner.png")) {
        cerr << "Failed to load politechnikaBaner.png" << endl;
    }
    sf::Sprite spritePolitechnikaBaner;
    spritePolitechnikaBaner.setTexture(texturePolitechnika);
    spritePolitechnikaBaner.setPosition(520.f, 22.f);
    spritePolitechnikaBaner.setScale(0.52f, 0.52f);

    //Bottom Line Nikname

    sf::Texture lineNickname;
    lineNickname.loadFromFile("assets/images/bottomLineNickname.png");

    sf::Sprite spriteLineNickname;
    spriteLineNickname.setTexture(lineNickname);
    spriteLineNickname.setScale(0.8f, 0.26f);
    spriteLineNickname.setPosition(0.f,232.f);

    sf::Sprite spriteLineCup;
    spriteLineCup.setTexture(bottomLine);
    spriteLineCup.setScale(0.1f,0.2f);
    spriteLineCup.setPosition(630.f, 244.f);

    //CUP
    sf::Texture cup;
    cup.loadFromFile("assets/images/cup.png");

    sf::Sprite spriteCup;
    spriteCup.setTexture(cup);
    spriteCup.setScale(0.06f, 0.06f);
    spriteCup.setPosition(575.f, 238.f);

    sf::Font font;
    font.loadFromFile("assets/fonts/CalSans-Regular.ttf");

    sf::Text trophiesText;
    trophiesText.setFont(font);
    trophiesText.setCharacterSize(24);
    trophiesText.setFillColor(sf::Color::White);
    trophiesText.setPosition(670.f, 244.f);

    // Coin Icon and Text
    sf::Texture coinTexture;
    coinTexture.loadFromFile("assets/icons/money.png");

    sf::Sprite spriteCoin;
    spriteCoin.setTexture(coinTexture);
    spriteCoin.setScale(0.08f, 0.08f);
    spriteCoin.setPosition(1180.f, 78.f); 

    sf::Text coinsText;
    coinsText.setFont(font);
    coinsText.setCharacterSize(24);
    coinsText.setFillColor(sf::Color::White);
    coinsText.setPosition(1230.f, 84.f);

    //Profile User

    vector<sf::Texture> banerTexture;
    sf::Texture baner1;
    baner1.loadFromFile("assets/icons/Frame1.png");

    banerTexture.push_back(baner1);
    UserProfile baner(&banerTexture[0], "User");

    //Cursor

    sf::Cursor cursorArrow;
    sf::Cursor cursorHand;
    cursorArrow.loadFromSystem(sf::Cursor::Arrow);
    cursorHand.loadFromSystem(sf::Cursor::Hand);


    // User profile banner
    sf::Texture bannerTexture;
    if (!bannerTexture.loadFromFile("assets/icons/Frame1.png")) {
        std::cerr << "Failed to load banner image!\n";
    }
    UserProfile profile(&bannerTexture, profileManager::getActiveUser());

    // Settings

    sf::Texture settingsTexture;
    settingsTexture.loadFromFile("assets/icons/Settings.png");

    sf::Sprite settingsButton;
    settingsButton.setTexture(settingsTexture);
    settingsButton.setScale(0.5f, 0.5f);
    settingsButton.setPosition(windowSize.x - 230.f, 110.f);

    // Setting
    SettingsWindow settings(window.getSize());
    bool logoutTriggered = false;
    bool showSettings = false;


    //Open/Close
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePosF(sf::Mouse::getPosition(window));

                if (settingsButton.getGlobalBounds().contains(mousePosF)) {
                    showSettings = !showSettings;
                }
            }

            
            if (showSettings) {
                settings.handleEvent(window, event, showSettings, logoutTriggered);
                if (logoutTriggered) {
                    profileManager::clearActiveUser();
                    window.close();
                    return;
                }
            }
        }

        sf::Vector2f mousePositionF(sf::Mouse::getPosition(window));
        if (spriteBtnBattle.getGlobalBounds().contains(mousePositionF)) {
            window.setMouseCursor(cursorHand);
        }
        else {
            window.setMouseCursor(cursorArrow);
        }


        window.clear(sf::Color::White);
        window.draw(spriteBackground);
        window.draw(spriteMiniArena);
        window.draw(spriteLeftCharacters);
        window.draw(spriteRightCharacters);
        window.draw(spriteBottomLine);
        window.draw(spriteBtnBattle);
        window.draw(spriteLineCup);
        window.draw(spriteLineNickname);
        window.draw(spriteCup);
        window.draw(settingsButton);
        profile.userDraw(window);

        for (int i = 0; i < buttons.size(); ++i) {
            buttons[i].setHoverEffect(buttons[i].isMouseOver(window));
            buttons[i].draw(window);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttons[i].isMouseOver(window)) {
                if (i == 0) {
                    string username = profileManager::getActiveUser();
                    showDeckWindow(window, username);
                }
                else if (i == 1) {
                   //battle 
                }
                else if (i == 2) {
                    showShopWindow(window);
                }
            }
        }

       

        

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && spriteBtnBattle.getGlobalBounds().contains(mousePositionF)) {
            string username = profileManager::getActiveUser();

            bool win = startGame(window, username);

            window.create(sf::VideoMode::getDesktopMode(), "Game V1", sf::Style::Fullscreen);
            window.setFramerateLimit(60);

            int coinsBefore = stoi(profileManager::loadUserData(username, "coins"));
            int trophiesBefore = stoi(profileManager::loadUserData(username, "trophies"));

            profileManager::updateBattleResult(username, win);

            int coinsAfter = stoi(profileManager::loadUserData(username, "coins"));
            int trophiesAfter = stoi(profileManager::loadUserData(username, "trophies"));

            int coinsDiff = coinsAfter - coinsBefore;
            int trophiesDiff = trophiesAfter - trophiesBefore;

            BattleResultWindow resultWindow(win, coinsDiff, trophiesDiff);
            resultWindow.show(window);
        }



        window.draw(spritePolitechnikaBaner);

        if (showSettings) {
            settings.draw(window);
        }

        std::string trophies = profileManager::loadUserData(profileManager::getActiveUser(), "trophies");
        std::string coins = profileManager::loadUserData(profileManager::getActiveUser(), "coins");
        trophiesText.setString(trophies);
        coinsText.setString(coins);

        window.draw(trophiesText);
        window.draw(spriteCoin);
        window.draw(coinsText);

        window.display();
    }
};