#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Tank.h"
#include "TankFactory.h"
#include "Bullet.h"


// Map representation
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int vidasP1 = 3;
int vidasP2 = 3;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Batalla de tanques", sf::Style::Default);

    TankFactory factory;

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error al cargar la fuente Arial.ttf" << std::endl;
        return -1;
    }

    // Cargar texturas
    sf::Texture tankTexture1, tankTexture2, groundTexture, wallTexture;
    if (!tankTexture1.loadFromFile("PLAYER01.png") || !tankTexture2.loadFromFile("PLAYER02.png") ||
        !groundTexture.loadFromFile("fondo.png") || !wallTexture.loadFromFile("pared.png")) {
        return -1; // Error al cargar las texturas
    }

    std::unique_ptr<Tank> player1 = factory.createTank(sf::Vector2f(100, 150), tankTexture1);
    std::unique_ptr<Tank> player2 = factory.createTank(sf::Vector2f(700, 500), tankTexture2);
    player2->rotate(180);

    std::vector<std::unique_ptr<Bullet>> bullets;
    
    std::unique_ptr<Bullet> bullet1 = nullptr;
    std::unique_ptr<Bullet> bullet2 = nullptr;


    sf::Clock clock;
    sf::Time elapsedTime;

    sf::Vector2f positionCurrentT1 = player1->getPosition();
    sf::Vector2f positionCurrentT2 = player2->getPosition();
    sf::Vector2f positionCurrentB1(0,0);
    sf::Vector2f positionCurrentB2(0,0);

    sf::Text text("", font, 20);
    
    text.setPosition(100, 5); 
    text.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Control Player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player1->moveForward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player1->moveBackward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player1->rotate(-100 * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player1->rotate(100 * deltaTime.asSeconds());

        // Control Player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player2->moveForward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player2->moveBackward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player2->rotate(-100 * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player2->rotate(100 * deltaTime.asSeconds());

        // Player 1 Shoot
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && elapsedTime.asSeconds() >= 1) {
            
            bullet1 = player1->shoot();
            
            elapsedTime = sf::Time::Zero; // Reiniciar el tiempo de espera
        }

        // Player 2 Shoot
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && elapsedTime.asSeconds() >= 1) {
            bullet2 = player2->shoot();
            elapsedTime = sf::Time::Zero; // Reiniciar el tiempo de espera
        }

        // Actualizar el tiempo transcurrido
        elapsedTime += deltaTime;

       
        if (bullet1) bullet1->update(deltaTime.asSeconds());
        if (bullet2) bullet2->update(deltaTime.asSeconds());

        // Render
        window.clear();

        for (int i = 0; i < MAP_HEIGHT; ++i) {
            for (int j = 0; j < MAP_WIDTH; ++j) {
                sf::Sprite sprite;
                if (map[i][j] == 0) {
                    sprite.setTexture(groundTexture);
                }
                else if (map[i][j] == 1) {
                    sprite.setTexture(wallTexture);
                }
                sprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                window.draw(sprite);
            }
        }

        window.draw(player1->getShape());
        window.draw(player2->getShape());
       
     
        if (bullet1 && bullet1->getPosition().x >= 0 && bullet1->getPosition().x <= window.getSize().x && bullet1->getPosition().y >= 0 && bullet1->getPosition().y <= window.getSize().y) {
            window.draw(bullet1->getShape());
        }
        if (bullet1) {
            positionCurrentB1 = bullet1->getPosition();
        }

        if (bullet2 && bullet2->getPosition().x >= 0 && bullet2->getPosition().x <= window.getSize().x && bullet2->getPosition().y >= 0 && bullet2->getPosition().y <= window.getSize().y) {
            window.draw(bullet2->getShape());
        }
        if (bullet2) {
            positionCurrentB2 = bullet2->getPosition();
        }
        positionCurrentT1 = player1->getPosition();
       
        sf::FloatRect player1Bounds = player1->getShape().getGlobalBounds();
         
        sf::FloatRect bullet2Bounds;
        if (bullet2) {
            bullet2Bounds = bullet2->getShape().getGlobalBounds();
        }
        else {
            bullet2Bounds = sf::FloatRect();
        }
        
        //Limites de player 2 con limite de bala 1
        sf::FloatRect player2Bounds = player2->getShape().getGlobalBounds();
        
        sf::FloatRect bullet1Bounds;
        if (bullet1) {
            bullet1Bounds = bullet1->getShape().getGlobalBounds();
        }
        else {
            bullet1Bounds = sf::FloatRect();
        }

        //Elimina la bala si hay impacto

        if (bullet2 && player1Bounds.intersects(bullet2Bounds)) {
            vidasP1--;
            bullet2.reset(); 
        }

        if (bullet1 && player2Bounds.intersects(bullet1Bounds)) {
            vidasP2--;
            bullet2.reset();
        }

        //si alguien gano
 
        if (vidasP2 == 0) {
            window.clear();
            text.setPosition(300, 250);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(30);
            text.setString("Ganador es Player 1");
            window.draw(text); // Asegúrate de dibujar el texto antes de mostrar la ventana
            window.display();

            // Bucle de eventos para mantener la ventana abierta
            sf::Event event;
            while (window.isOpen()) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
            }
        }
        else if (vidasP1 == 0) {
            window.clear();
            text.setPosition(300, 250);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            text.setString("Ganador es Player 2");
            window.draw(text);
            window.display();

            // Bucle de eventos para mantener la ventana abierta
            sf::Event event;
            while (window.isOpen()) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
            }
        }


        text.setString("Vidas P1: " + std::to_string(vidasP1) + "                      Vidas P2: " + std::to_string(vidasP2));
        window.draw(text);

        window.display();
    }

    return 0;
}
