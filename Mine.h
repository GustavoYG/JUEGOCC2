#ifndef MINE_H
#define MINE_H

#include <SFML/Graphics.hpp>

class Mine {
public:
    Mine(sf::Vector2f position, const sf::Texture& texture);

    void update(float deltaTime);
    sf::CircleShape getShape();
    sf::Vector2f getPosition() const;
    bool isActive() const;
    void activate();

private:
    sf::CircleShape shape;
    bool active;
    float activationTime;
    static constexpr float delayToActivate = 2.0f;
};

#endif // MINE_H

