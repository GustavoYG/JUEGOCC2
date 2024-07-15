#ifndef TANKFACTORY_H
#define TANKFACTORY_H

#include "Tank.h"
#include <memory>

class TankFactory {
public:
    std::unique_ptr<Tank> createTank(sf::Vector2f position, const sf::Texture& texture);
};

#endif // TANKFACTORY_H


