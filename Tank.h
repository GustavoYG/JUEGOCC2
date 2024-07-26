#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Mine.h"
#include <memory>

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;
const int TILE_SIZE = 40; // Definir TILE_SIZE aquí

class Tank {
public:
    Tank(sf::Vector2f position, const sf::Texture& texture);

    void moveForward(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]);
    void moveBackward(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]);
    void moveRight(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]);
    void moveLeft(float deltaTime, const sf::Vector2u& windowSize, const int map[MAP_HEIGHT][MAP_WIDTH]);
    void moveRight(float deltaTime);
    void moveLeft(float deltaTime);

    void rotate(float angle);

    std::unique_ptr<Mine> placeMine(const sf::Texture& mineTexture);

    
    std::unique_ptr<Bullet> shoot();
    sf::RectangleShape getShape();
    sf::Vector2f getPosition() const;
    void setPosition(float, float);

private:
    sf::RectangleShape shape;
    std::vector<std::unique_ptr<Mine>> mines;
    float speed;

    bool isWithinWindow(const sf::Vector2f& position, const sf::Vector2u& windowSize);
    bool checkCollisionWithWalls(const sf::Vector2f& newPosition, const int map[MAP_HEIGHT][MAP_WIDTH]);
};

#endif // TANK_H
