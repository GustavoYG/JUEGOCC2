#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(sf::Vector2f position, float rotation);

    void update(float deltaTime);
    sf::CircleShape getShape();
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;
};

#endif // BULLET_H

