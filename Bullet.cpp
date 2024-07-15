
#include "Bullet.h"
#include <cmath>

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}

Bullet::Bullet(sf::Vector2f position, float rotation) : speed(300.0f) {
    shape.setRadius(5.0f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
    shape.setOrigin(shape.getRadius(), shape.getRadius());

    float rad = rotation * (3.14159265f / 180.0f);
    velocity = sf::Vector2f(std::cos(rad), std::sin(rad)) * speed;
}

void Bullet::update(float deltaTime) {
    shape.move(velocity * deltaTime);
}

sf::CircleShape Bullet::getShape() {
    return shape;
}


