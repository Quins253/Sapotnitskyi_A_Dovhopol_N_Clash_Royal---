#include <SFML/Graphics.hpp>
#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "LoadingScreen.h"
#include "MainMenu.h"
#include "LoginScreen.h"
#include "ManagerProfile.h"
#include "Setting.h"

using namespace std;
   
sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
sf::RenderWindow window(desktop, "Clash Royal", sf::Style::Fullscreen);

sf::Music musicMainMenu;


int main()
{

    showLoadingScreen(window);

    //Music

    musicMainMenu.openFromFile("assets/audio/MenuMain_song.mp3");
    musicMainMenu.setLoop(true);
    musicMainMenu.play();

    if (!profileManager::activeUser()) {
        loginScreen(window);
    }

    showMainMenu(window);


    return 0;
}
