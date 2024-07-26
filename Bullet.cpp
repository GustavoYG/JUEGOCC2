
#include "Bullet.h"
#include <cmath>



Bullet::Bullet(sf::Vector2f position, float rotation) : speed(300.0f) {
    shape.setRadius(5.0f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
    shape.setOrigin(shape.getRadius(), shape.getRadius());

    float rad = rotation * (3.14159265f / 180.0f);
    velocity = sf::Vector2f(std::cos(rad), std::sin(rad)) * speed;
    
    if (!shotBuffer.loadFromFile("shotTank.mp3")) {
        std::cerr << "Error al cargar el sonido shotTank.mp3" << std::endl;
    }
    shotSound.setBuffer(shotBuffer);
    shotSound.play();
}

void Bullet::update(float deltaTime) {
    shape.move(velocity * deltaTime);
}

const sf::CircleShape& Bullet::getShape() const {
    return shape;
}
sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}


