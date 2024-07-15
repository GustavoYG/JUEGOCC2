#include "TankFactory.h"

std::unique_ptr<Tank> TankFactory::createTank(sf::Vector2f position, const sf::Texture& texture) {
    return std::make_unique<Tank>(position, texture);
}
