#include "Mine.h"

Mine::Mine(sf::Vector2f position, const sf::Texture& texture) : active(false), activationTime(0.0f) {
    shape.setRadius(10.0f);
    shape.setTexture(&texture);
    shape.setPosition(position);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
}

void Mine::update(float deltaTime) {
    if (!active) {
        activationTime += deltaTime;
        if (activationTime >= delayToActivate) {
            active = true;
        }
    }
}

sf::CircleShape Mine::getShape() {
    return shape;
}

sf::Vector2f Mine::getPosition() const {
    return shape.getPosition();
}

bool Mine::isActive() const {
    return active;
}

void Mine::activate() {
    active = true;
}
