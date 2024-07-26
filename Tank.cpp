#include "Tank.h"
#include <cmath>



sf::Vector2f Tank::getPosition() const {
    return shape.getPosition();
}

void Tank::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

Tank::Tank(sf::Vector2f position, const sf::Texture& texture) : speed(100.0f) {
    shape.setSize(sf::Vector2f(40, 40));
    shape.setTexture(&texture);
    shape.setPosition(position);
    shape.setOrigin(shape.getSize() / 2.0f);
}

void Tank::moveForward(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]) {
    float angle = shape.getRotation() * 3.14159265f / 180.0f;
    sf::Vector2f direction(std::cos(angle), std::sin(angle));
    sf::Vector2f newPosition = shape.getPosition() + direction * speed * deltaTime;

    if (isWithinWindow(newPosition, windowSize) && !checkCollisionWithWalls(newPosition, map)) {
        shape.setPosition(newPosition);
    }
}

void Tank::moveBackward(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]) {
    float angle = shape.getRotation() * 3.14159265f / 180.0f;
    sf::Vector2f direction(std::cos(angle), std::sin(angle));
    sf::Vector2f newPosition = shape.getPosition() - direction * speed * deltaTime;

    if (isWithinWindow(newPosition, windowSize) && !checkCollisionWithWalls(newPosition, map)) {
        shape.setPosition(newPosition);
    }
}

void Tank::moveRight(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]) {
    float angle = (shape.getRotation() + 90) * 3.14159265f / 180.0f;  
    sf::Vector2f direction(std::cos(angle), std::sin(angle));
    sf::Vector2f newPosition = shape.getPosition() + direction * speed * deltaTime;

    if (isWithinWindow(newPosition, windowSize) && !checkCollisionWithWalls(newPosition, map)) {
        shape.setPosition(newPosition);
    }
}


void Tank::moveLeft(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]) {
    float angle = (shape.getRotation()+90) * 3.14159265f / 180.0f;
    sf::Vector2f direction(std::cos(angle), std::sin(angle));
    sf::Vector2f newPosition = shape.getPosition() - direction * speed * deltaTime;

    if (isWithinWindow(newPosition, windowSize) && !checkCollisionWithWalls(newPosition, map)) {
        shape.setPosition(newPosition);
    }
}

void Tank::rotate(float angle) {
    shape.rotate(angle);
}

std::unique_ptr<Bullet> Tank::shoot() {
    sf::Vector2f position = shape.getPosition();
    float rotation = shape.getRotation();
    return std::make_unique<Bullet>(position, rotation);
}

sf::RectangleShape Tank::getShape() {
    return shape;
}

bool Tank::isWithinWindow(const sf::Vector2f& position, const sf::Vector2u& windowSize) {
    sf::FloatRect bounds = shape.getGlobalBounds();
    return (position.x - bounds.width / 2 > 0 && position.x + bounds.width / 2 < windowSize.x &&
        position.y - bounds.height / 2 > 0 && position.y + bounds.height / 2 < windowSize.y);
}

bool Tank::checkCollisionWithWalls(const sf::Vector2f& newPosition, const int map[MAP_HEIGHT][MAP_WIDTH]) {
    sf::FloatRect bounds = shape.getGlobalBounds();

    // Establece la nueva posición temporalmente
    bounds.left = newPosition.x - bounds.width / 2;
    bounds.top = newPosition.y - bounds.height / 2;

    int leftTile = static_cast<int>(bounds.left / TILE_SIZE);
    int rightTile = static_cast<int>((bounds.left + bounds.width) / TILE_SIZE);
    int topTile = static_cast<int>(bounds.top / TILE_SIZE);
    int bottomTile = static_cast<int>((bounds.top + bounds.height) / TILE_SIZE);

    // Asegúrate de que las coordenadas estén dentro del mapa
    leftTile = std::max(0, leftTile);
    rightTile = std::min(MAP_WIDTH - 1, rightTile);
    topTile = std::max(0, topTile);
    bottomTile = std::min(MAP_HEIGHT - 1, bottomTile);
    
    
    for (int i = topTile; i <= bottomTile; ++i) {
        for (int j = leftTile; j <= rightTile; ++j) {
            if (map[i][j] == 1) {
                return true;
            }
        }
    }
    return false;
}

std::unique_ptr<Mine> Tank::placeMine(const sf::Texture& mineTexture) {
    sf::Vector2f position = shape.getPosition();
    return std::make_unique<Mine>(position, mineTexture);
}

