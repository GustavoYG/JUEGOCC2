#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <iostream>
#include "Tank.h"
#include "TankFactory.h"
#include "Bullet.h"
#include "Mine.h"
#include <thread>
#include <atomic>
#include "CollisionHandler.h"
#include "TimerFunctor.h"


class Menu {
public:
    Menu();
    ~Menu();
    void showMenu();

    //void showEndMenu();
    

private:
    void handleKeyboardInput(sf::RenderWindow& window);
    void handleMouseInput(sf::RenderWindow& window);
    void updateMenuText();
    void startGameModeA();
    void startGameModeB();
    void playMusic();
    void stopMusic();
    void changeMusic(const std::string& filename);

    void showEndMenu(sf::RenderWindow& window, const std::string& winnerText);

    sf::Font font;
    sf::Text title;

    sf::Music musicMenu;
    std::thread musicThread;
    std::atomic<bool> playingMusic;

    sf::Text modeA;
    sf::Text modeB;
    sf::RectangleShape titleBackground;
    sf::RectangleShape modeABackground;
    sf::RectangleShape modeBBackground;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    int selectedOption;
};
void playImpact();
#endif // MENU_H
