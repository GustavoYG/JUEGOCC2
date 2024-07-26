#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class Bullet {
public:
    Bullet(sf::Vector2f position, float rotation);

    void update(float deltaTime);
    const sf::CircleShape& getShape() const;
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
    sf::SoundBuffer shotBuffer;
    sf::Sound shotSound;

    sf::Vector2f velocity;
    float speed;
};

#endif // BULLET_H

