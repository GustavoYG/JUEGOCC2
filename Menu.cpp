#include "menu.h"

sf::SoundBuffer bufferImpact;
sf::Sound sound;

sf::SoundBuffer bufferMine;
sf::Sound soundM;

TimerFunctor player1ShootTimer(2.0f);
TimerFunctor player2ShootTimer(2.0f);

Menu::Menu() : selectedOption(0), playingMusic(false) {
    if (!backgroundTexture.loadFromFile("menu.jpg")) {
        std::cerr << "Error al cargar la imagen menu.jpg" << std::endl;
        return;
    }
    sf::Vector2u backgroundTextureSize = backgroundTexture.getSize();
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setTextureRect(sf::IntRect(0, 0, backgroundTextureSize.x, backgroundTextureSize.y));

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error al cargar la fuente Arial.ttf" << std::endl;
        return;
    }
    if (!musicMenu.openFromFile("menu_sound.mp3")) {
        std::cerr << "Error al cargar la fuente menu_sound.mp3" << std::endl;
        return;
    }
    if (!bufferImpact.loadFromFile("explosion.mp3")) {
        std::cerr << "Error al cargar el archivo de sonido de impacto" << std::endl;
    }
    if (!bufferMine.loadFromFile("mineSound.mp3")) {
        std::cerr << "Error al cargar el archivo de sonido de impacto" << std::endl;
    }

 

    title.setFont(font);
    title.setString("Selecciona el modo de juego");
    title.setCharacterSize(30);
    title.setPosition(250, 50);
    title.setFillColor(sf::Color::White);

    modeA.setFont(font);
    modeA.setString("VS MODE");
    modeA.setCharacterSize(25);
    modeA.setPosition(220, 500);
    modeA.setFillColor(sf::Color::Black);
    modeA.setOutlineThickness(2); // Grosor del borde del texto
    modeA.setOutlineColor(sf::Color::Black);

    modeB.setFont(font);
    modeB.setString("SUPERVIVENCIA");
    modeB.setCharacterSize(25);
    modeB.setPosition(520, 500);
    modeB.setFillColor(sf::Color::Black);
    modeB.setOutlineThickness(2); // Grosor del borde del texto
    modeB.setOutlineColor(sf::Color::Black);

    titleBackground.setSize(sf::Vector2f(title.getGlobalBounds().width, title.getGlobalBounds().height));
    titleBackground.setPosition(title.getPosition());
    titleBackground.setFillColor(sf::Color::Black);

    modeABackground.setSize(sf::Vector2f(200, modeA.getGlobalBounds().height + 20));
    modeABackground.setPosition(modeA.getPosition().x - 50, modeA.getPosition().y);
    modeABackground.setFillColor(sf::Color(119, 136, 153));
    modeABackground.setOutlineThickness(5);
    modeABackground.setOutlineColor(sf::Color::Black);

    modeBBackground.setSize(sf::Vector2f(250, modeB.getGlobalBounds().height + 20));
    modeBBackground.setPosition(modeB.getPosition().x - 15, modeB.getPosition().y);
    modeBBackground.setFillColor(sf::Color(119, 136, 153));
    modeBBackground.setOutlineThickness(5);
    modeBBackground.setOutlineColor(sf::Color::Black);
}

Menu::~Menu() {
    // stop music and wait to end thread
    stopMusic();
}
void Menu::playMusic() {
    while (playingMusic) {
        if (musicMenu.getStatus() == sf::Music::Stopped) {
            musicMenu.play();
        }
        sf::sleep(sf::seconds(0.1f)); // Para evitar un bucle ajustado
    }
    musicMenu.stop();
}
void Menu::stopMusic() {
    playingMusic = false;
    if (musicThread.joinable()) {
        musicThread.join();
    }
}

void Menu::changeMusic(const std::string& filename) {
    stopMusic();
    if (!musicMenu.openFromFile(filename)) {
        std::cerr << "Error al cargar la musica " << filename << std::endl;
    }
    else {
        playingMusic = true;
        musicThread = std::thread(&Menu::playMusic, this);
    }
    musicMenu.setVolume(40);
}
void Menu::showMenu() {
    sf::RenderWindow window(sf::VideoMode(900, 700), "Menú de selección");

    // Obtener el tamaño de la ventana
    sf::Vector2u windowSize = window.getSize();

    // Calcular la escala necesaria para ajustar la imagen al tamaño de la ventana
    float scaleX = static_cast<float>(windowSize.x) / backgroundSprite.getTexture()->getSize().x;
    float scaleY = static_cast<float>(windowSize.y) / backgroundSprite.getTexture()->getSize().y;

    // Aplicar la escala al sprite
    backgroundSprite.setScale(scaleX, scaleY);

    // Start muscic on a thread
    playingMusic = true;
    musicThread = std::thread(&Menu::playMusic, this);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            handleKeyboardInput(window);
            handleMouseInput(window);
        }

        window.clear();
        window.draw(backgroundSprite);

        // Dibujar los rectángulos detrás del texto
        window.draw(titleBackground);
        window.draw(modeABackground);
        window.draw(modeBBackground);

        updateMenuText();

        window.draw(title);
        window.draw(modeA);
        window.draw(modeB);

        window.display();
    }
    // Detener la música cuando se cierre la ventana
    stopMusic();
}


void Menu::handleKeyboardInput(sf::RenderWindow& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        selectedOption = (selectedOption == 0) ? 1 : 0;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        selectedOption = (selectedOption == 1) ? 0 : 1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        selectedOption = 0;
        stopMusic();
        window.close();
        startGameModeA(); // Llama al modo de juego A
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        selectedOption = 1;
        stopMusic();
        window.close();
        startGameModeB(); // Llama al modo de juego B
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (selectedOption == 0) {
            stopMusic();
            window.close();
            startGameModeA(); // Llama al modo de juego A
        }
        else if (selectedOption == 1) {
            stopMusic();
            window.close();
            startGameModeB(); // Llama al modo de juego B
        }
    }
}


void Menu::handleMouseInput(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        if (modeA.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
            selectedOption = 0;
            stopMusic();
            window.close();
            startGameModeA();
        }
        else if (modeB.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
            selectedOption = 1;
            stopMusic();
            window.close();
            startGameModeB();
        }
    }
}

void Menu::updateMenuText() {
    modeA.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
    modeB.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
}


void playImpact() {

    sound.setBuffer(bufferImpact); // Asignar el buffer de sonido

    // Reproducir el sonido
    sound.play();
}
void playMine() {

    soundM.setBuffer(bufferMine); // Asignar el buffer de sonido

    // Reproducir el sonido
    soundM.play();
}

// Implementación del modo de juego A

void Menu::startGameModeA() {
    const int MAP_HEIGHT = 15;
    const int MAP_WIDTH = 20;
    const int TILE_SIZE = 40;
    int map[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1},
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

    //mines
    sf::Clock mineClock1, mineClock2; // Clocks for mine placement cooldowns
    std::vector<std::unique_ptr<Mine>> mines1; // Mines placed by player 1
    std::vector<std::unique_ptr<Mine>> mines2; // Mines placed by player 2



    std::unique_ptr<Bullet> bullet1 = nullptr;
    std::unique_ptr<Bullet> bullet2 = nullptr;

    sf::Clock clock;
    //sf::Time elapsedTime, elapsedTime2;

    sf::Vector2f positionCurrentT1 = player1->getPosition();
    sf::Vector2f positionCurrentT2 = player2->getPosition();
    sf::Vector2f positionCurrentB1(0, 0);
    sf::Vector2f positionCurrentB2(0, 0);

    sf::Text text("", font, 20);
    text.setPosition(100, 5);
    text.setFillColor(sf::Color::Black);

    changeMusic("soundGameModeA.mp3");

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && mines1.size() < 3 && mineClock1.getElapsedTime().asSeconds() >= 3.0f) {
            mines1.push_back(player1->placeMine(mineTexture));
            playMine();
            mineClock1.restart();
        }
            


        // Control Player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
            player2->moveForward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
            player2->moveBackward(deltaTime.asSeconds(), window.getSize(), map);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
            player2->rotate(-100 * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
            player2->rotate(100 * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7) && mines2.size() < 3 && mineClock2.getElapsedTime().asSeconds() >= 3.0f) {
            mines2.push_back(player2->placeMine(mineTexture));
            playMine();
            mineClock2.restart();
        }
           

        // Player 1 Shoot
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player1ShootTimer()){
            bullet1 = player1->shoot();
            
        }

        // Player 2 Shoot
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && player2ShootTimer()){
            bullet2 = player2->shoot();
            
        }

        // Actualizar balas
        if (bullet1) bullet1->update(deltaTime.asSeconds());
        if (bullet2) bullet2->update(deltaTime.asSeconds());

        // Actualizar minas
         for (auto& mine : mines1) {
            mine->update(deltaTime.asSeconds());
        }

        for (auto& mine : mines2) {
            mine->update(deltaTime.asSeconds());
        }

        // Verificar colisiones con minas
        CollisionHandler::handlePlayerMineCollisions(mines1, player1, vidasP1);
        CollisionHandler::handlePlayerMineCollisions(mines1, player2, vidasP2);
        CollisionHandler::handlePlayerMineCollisions(mines2, player1, vidasP1);
        CollisionHandler::handlePlayerMineCollisions(mines2, player2, vidasP2);

        // Verificar colisiones entre balas y minas
        CollisionHandler::handleBulletMineCollisions(mines1, bullet1);
        CollisionHandler::handleBulletMineCollisions(mines1, bullet2);
        CollisionHandler::handleBulletMineCollisions(mines2, bullet1);
        CollisionHandler::handleBulletMineCollisions(mines2, bullet2);

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
        for (const auto& mine : mines1) {
            window.draw(mine->getShape());
        }
        for (const auto& mine : mines2) {
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
            playImpact();
            vidasP1--;
            
            bullet2.reset();
        }

        if (bullet1 && player2Bounds.intersects(bullet1Bounds)) {
            playImpact();
            vidasP2--;
            
            bullet1.reset();
        }

        //si alguien gano
        
        if (vidasP2 <= 0) {
            stopMusic();
            showEndMenu(window, "Ganador es Player 1");
        }
        else if (vidasP1 <= 0) {
            stopMusic();
            showEndMenu(window, "Ganador es Player 2");
        }
        
        text.setString("Vidas P1: " + std::to_string(vidasP1) + "                      Vidas P2: " + std::to_string(vidasP2));
        window.draw(text);

        window.display();
        
    }

}


void Menu::startGameModeB() {
    // Implementación del modo de juego B aquí...
    {
        const int MAP_HEIGHT = 15;
        const int MAP_WIDTH = 20;
        const int TILE_SIZE = 40;
        int map[MAP_HEIGHT][MAP_WIDTH] = {
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 1, 1, 1, 0, 0,0, 0, 1, 1, 1, 1, 1, 0, 0, 1},
        };

        int vidasP1 = 3;
        int vidasP2 = 3;
        int vidasM1 = 5;
        int vidasM2 = 5;

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

        megaT1->rotate(90);
        megaT2->rotate(90);

        std::unique_ptr<Bullet> bullet1 = nullptr;
        std::unique_ptr<Bullet> bullet2 = nullptr;

        std::unique_ptr<Bullet> bulletM1 = nullptr;
        std::unique_ptr<Bullet> bulletM2 = nullptr;

        float megaSpeed1 = 50.0f; // Velocidad inicial de megaT1
        float megaSpeed2 = 50.0f; // Velocidad inicial de megaT2
        bool movingRight1 = true;
        bool movingRight2 = false;


        sf::Clock clock;
        //sf::Time elapsedTime;
        //sf::Time elapsedTime2;
        sf::Clock shootClock1;//define shoot time
        sf::Clock shootClock2;

       
        sf::Text text("", font, 20);
        sf::Text textM("", font, 20);
        text.setPosition(150, 560);
        text.setFillColor(sf::Color::Black);
        textM.setPosition(150, 5);
        textM.setFillColor(sf::Color::White);
        
        changeMusic("soundGameModeB.mp3");

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

                //stop move player
                if (player1->getPosition().x > window.getSize().x / 2 - TILE_SIZE / 2)
                    player1->setPosition(window.getSize().x / 2 - TILE_SIZE / 2, player1->getPosition().y);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player1->moveRight(deltaTime.asSeconds(), window.getSize(), map);
                if (player1->getPosition().x > window.getSize().x / 2 - TILE_SIZE / 2)
                    player1->setPosition(window.getSize().x / 2 - TILE_SIZE / 2, player1->getPosition().y);
            }



            // Control Player 2
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player1ShootTimer()){ //elapsedTime.asSeconds() >= 2) {
                bullet1 = player1->shoot();
                
            }

            // Player 2 Shoot
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && player2ShootTimer()) {
                bullet2 = player2->shoot();
            }

            // Actualizar el tiempo transcurrido
            //elapsedTime += deltaTime; ahora se usa el functor player1Shoot, declarado como dato general

            // Actualizar balas velocity
            if (bullet1) bullet1->update(deltaTime.asSeconds());
            if (bullet2) bullet2->update(deltaTime.asSeconds());

            // Movimiento automático de megaT1
            if (movingRight1) {
                megaT1->moveLeft(deltaTime.asSeconds(), window.getSize(), map);
                if (megaT1->getPosition().x + TILE_SIZE > window.getSize().x / 2) {
                    movingRight1 = false;
                }
            }

            else {
                megaT1->moveRight(deltaTime.asSeconds(), window.getSize(), map);
                if (megaT1->getPosition().x < TILE_SIZE) {
                    movingRight1 = true;
                }
            }

            // Movimiento automático de megaT2
            if (movingRight2) {
                megaT2->moveLeft(deltaTime.asSeconds(), window.getSize(), map);
                if (megaT2->getPosition().x + TILE_SIZE > window.getSize().x) {
                    movingRight2 = false;
                }
            }
            else {
                megaT2->moveRight(deltaTime.asSeconds(), window.getSize(), map);
                if (megaT2->getPosition().x < window.getSize().x / 2 + TILE_SIZE) {
                    movingRight2 = true;
                }
            }

            // Disparos automáticos de megaT1 y megaT2
            if (shootClock1.getElapsedTime().asSeconds() >= 3) {
                bulletM1 = megaT1->shoot();
                shootClock1.restart();
            }

            if (shootClock2.getElapsedTime().asSeconds() >= 3) {
                bulletM2 = megaT2->shoot();
                shootClock2.restart();
            }

            // Actualizar balas de megaT1 y megaT2
            if (bulletM1) bulletM1->update(deltaTime.asSeconds());
            if (bulletM2) bulletM2->update(deltaTime.asSeconds());


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

                      
            // draw bullets
            //player
            if (bullet1 && bullet1->getPosition().x >= 0 && bullet1->getPosition().x <= window.getSize().x/2 && bullet1->getPosition().y >= 0 && bullet1->getPosition().y <= window.getSize().y) {
                window.draw(bullet1->getShape());
            }
            

            if (bullet2 && bullet2->getPosition().x >= 0 && bullet2->getPosition().x <= window.getSize().x && bullet2->getPosition().y >= 0 && bullet2->getPosition().y <= window.getSize().y) {
                window.draw(bullet2->getShape());
            }
            

            //bot
            if (bulletM1 && bulletM1->getPosition().x >= 0 && bulletM1->getPosition().x <= window.getSize().x / 2 && bulletM1->getPosition().y >= 0 && bulletM1->getPosition().y <= window.getSize().y) {
                window.draw(bulletM1->getShape());
            }
            
            if (bulletM2 && bulletM2->getPosition().x >= 0 && bulletM2->getPosition().x <= window.getSize().x && bulletM2->getPosition().y >= 0 && bulletM2->getPosition().y <= window.getSize().y) {
                window.draw(bulletM2->getShape());
            }
                        
             //Bounds player1 with bulletM1 - Bound Mega1 with bullet1

            sf::FloatRect player1Bounds = player1->getShape().getGlobalBounds();
            sf::FloatRect megaT1Bounds = megaT1->getShape().getGlobalBounds();

            sf::FloatRect bulletM1Bounds;
            sf::FloatRect bullet1Bounds;

            if (bulletM1) {
                bulletM1Bounds = bulletM1->getShape().getGlobalBounds();
            }
            
            if (bullet1) {
                bullet1Bounds = bullet1->getShape().getGlobalBounds();
            }
            

            //Bounds player2 with bulletM2 - Bound Mega2 with bullet2

            sf::FloatRect player2Bounds = player2->getShape().getGlobalBounds();
            sf::FloatRect megaT2Bounds = megaT2->getShape().getGlobalBounds();

            sf::FloatRect bulletM2Bounds;
            sf::FloatRect bullet2Bounds;

            if (bulletM2) {
                bulletM2Bounds = bulletM2->getShape().getGlobalBounds();
            }
           
            if (bullet2) {
                bullet2Bounds = bullet2->getShape().getGlobalBounds();
            }
            


            //Elimina la bala si hay impacto

            if (player1Bounds.intersects(bulletM1Bounds)) {
                playImpact();
                vidasP1--;
                bulletM1.reset();
            }

            if (megaT1Bounds.intersects(bullet1Bounds)) {
                playImpact();
                vidasM1--;
                bullet1.reset();
            }


            if (player2Bounds.intersects(bulletM2Bounds)) {
                playImpact();
                vidasP2--;
                bulletM2.reset();
            }

            if (megaT2Bounds.intersects(bullet2Bounds)) {
                playImpact();
                vidasM2--;
                bullet2.reset();
            }
            
           

            //si alguien gano


            if (vidasP2 == 0) {
                stopMusic();
                showEndMenu(window, "Ganador es Player 1");
            }
            else if (vidasP1 == 0) {
                stopMusic();
                showEndMenu(window, "Ganador es Player 2");
            }else if (vidasM1==0) {
                stopMusic();
                showEndMenu(window, "Ganador es Player 1");
            }else if (vidasM2 == 0) {
                stopMusic();
                showEndMenu(window, "Ganador es Player 2");
            }

            textM.setString("Vidas MT1: " + std::to_string(vidasM1) + "                                Vidas MT2: " + std::to_string(vidasM2));
            window.draw(textM);


            text.setString("Vidas P1: " + std::to_string(vidasP1) + "                                  Vidas P2: " + std::to_string(vidasP2));
            window.draw(text);

            window.display();
        }
    }
}

void Menu::showEndMenu(sf::RenderWindow& window, const std::string& winnerText) {
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error al cargar la fuente Arial.ttf" << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(winnerText);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setPosition(300, 250);

    sf::Text option1, option2, option3;
    option1.setFont(font);
    option1.setString("1. Finalizar juego");
    option1.setCharacterSize(24);
    option1.setFillColor(sf::Color::White);
    option1.setPosition(300, 350);

    option2.setFont(font);
    option2.setString("2. Volver a jugar otra partida");
    option2.setCharacterSize(24);
    option2.setFillColor(sf::Color::White);
    option2.setPosition(300, 400);

    option3.setFont(font);
    option3.setString("3. Volver al menú principal");
    option3.setCharacterSize(24);
    option3.setFillColor(sf::Color::White);
    option3.setPosition(300, 450);

    int selectedOption = 0;

    while (window.isOpen()) {
        window.clear();
        window.draw(text);
        window.draw(option1);
        window.draw(option2);
        window.draw(option3);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    window.close();
                    startGameModeA(); // o startGameModeB();
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    window.close();
                    Menu menu;
                    menu.showMenu();
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    selectedOption = (selectedOption - 1 + 3) % 3;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedOption = (selectedOption + 1) % 3;
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedOption == 0) {
                        window.close();
                    }
                    else if (selectedOption == 1) {
                        window.close();
                        startGameModeA(); // o startGameModeB();
                    }
                    else if (selectedOption == 2) {
                        window.close();
                        Menu menu;
                        menu.showMenu();
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (option1.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    window.close();
                }
                else if (option2.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    window.close();
                    startGameModeA(); // o startGameModeB();
                }
                else if (option3.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    window.close();
                    Menu menu;
                    menu.showMenu();
                }
            }
        }

        option1.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
        option2.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
        option3.setFillColor(selectedOption == 2 ? sf::Color::Yellow : sf::Color::White);
    }
}