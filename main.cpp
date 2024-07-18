#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "Tank.h"
#include "TankFactory.h"
#include "Bullet.h"
#include "Mine.h"

// Funciones para manejar los modos de juego
void startGameModeA();
void startGameModeB();

// Mostrar el menú
void showMenu() {
    sf::RenderWindow window(sf::VideoMode(900, 700), "Menú de selección");

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("menu.jpg")) {
        std::cerr << "Error al cargar la imagen menu.jpg" << std::endl;
        return;
    }
    sf::Vector2u backgroundTextureSize = backgroundTexture.getSize();
    sf::Sprite backgroundSprite(backgroundTexture,sf::IntRect(0,0, backgroundTextureSize.x, backgroundTextureSize.y));

    // Obtener el tamaño de la ventana
    sf::Vector2u windowSize = window.getSize();

    // Calcular la escala necesaria para ajustar la imagen al tamaño de la ventana
    float scaleX = static_cast<float>(windowSize.x) / backgroundTextureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / backgroundTextureSize.y;

    // Aplicar la escala al sprite
    backgroundSprite.setScale(scaleX, scaleY);

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error al cargar la fuente Arial.ttf" << std::endl;
        return;
    }

    sf::Text title("Selecciona el modo de juego", font, 30);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::White);

    sf::Text modeA("1. Modo de Juego A", font, 24);
    modeA.setPosition(250, 150);
    modeA.setFillColor(sf::Color::White);

    sf::Text modeB("2. Modo de Juego B", font, 24);
    modeB.setPosition(250, 250);
    modeB.setFillColor(sf::Color::White);

    // Crear rectángulos de fondo para el texto
    sf::RectangleShape titleBackground(sf::Vector2f(title.getGlobalBounds().width, title.getGlobalBounds().height));
    titleBackground.setPosition(title.getPosition());
    titleBackground.setFillColor(sf::Color::Black);

    sf::RectangleShape modeABackground(sf::Vector2f(modeA.getGlobalBounds().width, modeA.getGlobalBounds().height));
    modeABackground.setPosition(modeA.getPosition());
    modeABackground.setFillColor(sf::Color::Black);

    sf::RectangleShape modeBBackground(sf::Vector2f(modeB.getGlobalBounds().width, modeB.getGlobalBounds().height));
    modeBBackground.setPosition(modeB.getPosition());
    modeBBackground.setFillColor(sf::Color::Black);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    window.close();
                    startGameModeA(); // Llama al modo de juego A
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    window.close();
                    startGameModeB(); // Llama al modo de juego B
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        // Dibujar los rectángulos detrás del texto
        window.draw(titleBackground);
        window.draw(modeABackground);
        window.draw(modeBBackground);

        window.draw(title);
        window.draw(modeA);
        window.draw(modeB);
        window.display();
    }
}

// Implementación del modo de juego A
void startGameModeA() {
    const int MAP_HEIGHT = 15;
    const int MAP_WIDTH = 20;
    const int TILE_SIZE = 40;
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

    sf::RenderWindow window(sf::VideoMode(800, 600), "Batalla de tanques", sf::Style::Default);
    TankFactory factory;

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error al cargar la fuente Arial.ttf" << std::endl;
        return;
    }

    sf::Texture tankTexture1, tankTexture2, groundTexture, wallTexture, mineTexture;
    if (!tankTexture1.loadFromFile("PLAYER01.png") || !tankTexture2.loadFromFile("PLAYER02.png") ||
        !groundTexture.loadFromFile("fondo.png") || !wallTexture.loadFromFile("pared.png") || !mineTexture.loadFromFile("mine.png")) {
        std::cerr << "Error al cargar las texturas" << std::endl;
        return;
    }

    std::unique_ptr<Tank> player1 = factory.createTank(sf::Vector2f(100, 150), tankTexture1);
    std::unique_ptr<Tank> player2 = factory.createTank(sf::Vector2f(700, 500), tankTexture2);
    player2->rotate(180);

    //std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Mine>> mines;

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) 
            mines.push_back(player1->placeMine(mineTexture));
        

        // Control Player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
            player2->moveForward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
            player2->moveBackward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
            player2->rotate(-100 * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
            player2->rotate(100 * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
            mines.push_back(player2->placeMine(mineTexture));

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

        // Actualizar balas
        if (bullet1) bullet1->update(deltaTime.asSeconds());
        if (bullet2) bullet2->update(deltaTime.asSeconds());

        // Actualizar minas
        for (auto& mine : mines) {
            mine->update(deltaTime.asSeconds());
        }

        // Verificar colisiones con minas
        for (auto it = mines.begin(); it != mines.end(); ) {
            if ((*it)->isActive()) {
                if ((*it)->getShape().getGlobalBounds().intersects(player1->getShape().getGlobalBounds())) {
                    vidasP1-2;
                    it = mines.erase(it);
                }
                else if ((*it)->getShape().getGlobalBounds().intersects(player2->getShape().getGlobalBounds())) {
                    vidasP2-2;
                    it = mines.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }


        // Render
        window.clear();

        // Dibujar el mapa
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
        
        // draw bullets
       
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

        // draw mines
        for (const auto& mine : mines) {
            window.draw(mine->getShape());
        }

        //Limites de player 1 con limite de bala 2

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

        if (vidasP2 <= 0) {
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
        else if (vidasP1 <= 0) {
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
    }


void startGameModeB() {
    // Implementación del modo de juego B aquí...
    {
        const int MAP_HEIGHT = 15;
        const int MAP_WIDTH = 20;
        const int TILE_SIZE = 40;
        int map[MAP_HEIGHT][MAP_WIDTH] = {
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        };

        int vidasP1 = 3;
        int vidasP2 = 3;
        int vidasM1 = 10;
        int vidasM2 = 10;

        sf::RenderWindow window(sf::VideoMode(800, 600), "Duelo de tanques", sf::Style::Default);
        TankFactory factory;

        sf::Font font;
        if (!font.loadFromFile("Arial.ttf")) {
            std::cerr << "Error al cargar la fuente Arial.ttf" << std::endl;
            return;
        }

        sf::Texture tankTexture1, tankTexture2, groundTexture, wallTexture, megaTankTexture;
        if (!tankTexture1.loadFromFile("PLAYER01.png") || !tankTexture2.loadFromFile("PLAYER02.png") ||
            !groundTexture.loadFromFile("fondo.png") || !wallTexture.loadFromFile("pared.png") || !megaTankTexture.loadFromFile("megatank.jpg")) {
            std::cerr << "Error al cargar las texturas" << std::endl;
            return;
        }

        std::unique_ptr<Tank> player1 = factory.createTank(sf::Vector2f(225, 500), tankTexture1);
        std::unique_ptr<Tank> player2 = factory.createTank(sf::Vector2f(675, 500), tankTexture2);
        std::unique_ptr<Tank> megaT1 = factory.createTank(sf::Vector2f(225, 200), megaTankTexture);
        std::unique_ptr<Tank> megaT2 = factory.createTank(sf::Vector2f(675, 200), megaTankTexture);
        player1->rotate(-90);
        player2->rotate(-90);

        std::unique_ptr<Bullet> bullet1 = nullptr;
        std::unique_ptr<Bullet> bullet2 = nullptr;

        std::unique_ptr<Bullet> bulletM1 = nullptr;
        std::unique_ptr<Bullet> bulletM2 = nullptr;

        sf::Clock clock;
        sf::Time elapsedTime;

        sf::Vector2f positionCurrentT1 = player1->getPosition();
        sf::Vector2f positionCurrentT2 = player2->getPosition();
        sf::Vector2f positionCurrentB1(0, 0);
        sf::Vector2f positionCurrentB2(0, 0);

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player1->moveLeft(deltaTime.asSeconds(), window.getSize(), map);
                if (player1->getPosition().x > window.getSize().x / 2 - TILE_SIZE / 2)
                    player1->setPosition(window.getSize().x / 2 - TILE_SIZE / 2, player1->getPosition().y);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                player1->moveRight(deltaTime.asSeconds(), window.getSize(), map);
                if (player1->getPosition().x > window.getSize().x / 2 - TILE_SIZE / 2) 
                player1->setPosition(window.getSize().x / 2 - TILE_SIZE / 2, player1->getPosition().y);
            }
            


            // Control Player 2
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)){
                player2->moveLeft(deltaTime.asSeconds(), window.getSize(), map);
                if (player2->getPosition().x < window.getSize().x / 2 + TILE_SIZE / 2)
                    player2->setPosition(window.getSize().x / 2 + TILE_SIZE / 2, player2->getPosition().y);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
                player2->moveRight(deltaTime.asSeconds(), window.getSize(), map);
                if (player2->getPosition().x < window.getSize().x / 2 + TILE_SIZE / 2)
                    player2->setPosition(window.getSize().x / 2 + TILE_SIZE / 2, player2->getPosition().y);
            }

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

            // Actualizar balas
            if (bullet1) bullet1->update(deltaTime.asSeconds());
            if (bullet2) bullet2->update(deltaTime.asSeconds());

            
            // Render
            window.clear();

            // Dibujar el mapa
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
            window.draw(megaT1->getShape());
            window.draw(megaT2->getShape());


            // draw line in the middle

            sf::RectangleShape divider(sf::Vector2f(2, window.getSize().y));
            divider.setPosition(window.getSize().x / 2, 0);
            divider.setFillColor(sf::Color::Red);
            window.draw(divider);

            window.draw(player1->getShape());
            window.draw(player2->getShape());

            // draw bullets

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

           
            //Limites de player 1 con limite de bala MEga 1

            sf::FloatRect player1Bounds = player1->getShape().getGlobalBounds();

            sf::FloatRect bulletM1Bounds;
            if (bulletM1) {
                bulletM1Bounds = bulletM1->getShape().getGlobalBounds();
            }
            else {
                bulletM1Bounds = sf::FloatRect();
            }

            //Limites de player 2 con limite de bala mega 2
            sf::FloatRect player2Bounds = player2->getShape().getGlobalBounds();

            sf::FloatRect bulletM2Bounds;
            if (bullet1) {
                bulletM2Bounds = bulletM2->getShape().getGlobalBounds();
            }
            else {
                bulletM2Bounds = sf::FloatRect();
            }

            //Elimina la bala si hay impacto

            if (bullet2 && player1Bounds.intersects(bulletM2Bounds)) {
                vidasP1--;
                bullet2.reset();
            }

            if (bullet1 && player2Bounds.intersects(bulletM1Bounds)) {
                vidasP2--;
                bullet2.reset();
            }

            //si alguien gano

            if (vidasP2 <= 0) {
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
            else if (vidasP1 <= 0) {
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
    }

}

int main() {
    showMenu();
    return 0;
}
