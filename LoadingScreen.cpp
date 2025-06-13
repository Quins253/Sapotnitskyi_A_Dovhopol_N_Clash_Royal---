#include <iostream>
#include "LoadingScreen.h"

void showLoadingScreen(sf::RenderWindow& window) {

    //Create Sprite, SpriteSize
    sf::Texture texture;
    texture.loadFromFile("assets/images/clash-royale_start.jpg");


    sf::Sprite spriteMenu;
    spriteMenu.setTexture(texture);
    spriteMenu.setPosition(0.f, 0.f);
    spriteMenu.setScale(0.2f, 0.2f);

    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaletX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaletY = static_cast<float>(windowSize.y) / textureSize.y;
    spriteMenu.setScale(scaletX, scaletY);


    //Progress bar

    sf::RectangleShape progressBarBack;
    sf::RectangleShape progressBarFront;
    
    sf::Vector2f barSize(windowSize.x * 0.6f, 30.0f);
    float marginBottom = 50.f;

    progressBarBack.setSize(barSize);
    progressBarBack.setFillColor(sf::Color::Black);
    progressBarBack.setPosition((windowSize.x - barSize.x) / 2.f, windowSize.y - marginBottom - barSize.y);

    progressBarFront.setSize(sf::Vector2f(0.f, barSize.y));
    progressBarFront.setFillColor(sf::Color::Blue);
    progressBarFront.setPosition(progressBarBack.getPosition());


    //Time Progress Bar
    sf::Font font;
    font.loadFromFile("assets/fonts/CalSans-Regular.ttf");

    sf::Text textLoading;
    textLoading.setFont(font);
    textLoading.setCharacterSize(24);
    textLoading.setFillColor(sf::Color::White);
    textLoading.setPosition(
        windowSize.x / 2.f,
        progressBarBack.getPosition().y + progressBarBack.getSize().y + 10.f
    );
    textLoading.setOrigin(textLoading.getLocalBounds().width / 2.f, textLoading.getLocalBounds().height / 2.f);


    sf::Clock timeBarStart;
    sf::Clock deltaClock;

    float loadingStart = 5.0f;
    float progress = 0.0f;
    float speedProgress = 17.5f;



    //Open/Close, +Timer Progress Bar
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        float deltaTime = deltaClock.restart().asSeconds();
        float elapsed = timeBarStart.getElapsedTime().asSeconds();
        
        if (elapsed > loadingStart)
        {
            progress += speedProgress * deltaTime;
            if (progress > 100.f)
                progress = 100.f;
        }

        if (progress >= 100) {
            sf::sleep(sf::seconds(2));
            return;
        }

        progressBarFront.setSize(sf::Vector2f(barSize.x * (progress / 100.f), barSize.y));
        textLoading.setString(std::to_string(static_cast<int>(progress)) + "%");
        textLoading.setOrigin(textLoading.getLocalBounds().width / 2.f, textLoading.getLocalBounds().height / 2.f);
        textLoading.setPosition(
            windowSize.x / 2.f,
            progressBarBack.getPosition().y + progressBarBack.getSize().y + 10.f
        );


        window.clear(sf::Color::White);
        window.draw(spriteMenu);
        window.draw(progressBarBack);
        window.draw(progressBarFront);
        window.draw(textLoading);
        window.display();
    }
}
