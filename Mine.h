#ifndef MINE_H
#define MINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <iostream>

class Mine {
public:
    Mine(sf::Vector2f position, const sf::Texture& texture);
    //Mine(sf::Vector2f position, sf::Texture& texture);

    void update(float deltaTime);
    sf::CircleShape getShape();
    sf::Vector2f getPosition() const;
    bool isActive() const;
    void activate();
    
    /*const sf::Sprite& getShape() const;
    sf::Vector2f getPosition() const;
    bool isActive() const;
    bool isExploded() const;
    void explode();
    */
private:

    //sf::Sprite shape;
    //bool active;
    //bool exploded;
    //float activationTime;
    //const float activationDelay = 3.0f;
    //const float explosionRadius = 50.0f;

    sf::CircleShape shape;
    bool active;
    float activationTime;
    static constexpr float delayToActivate = 2.0f;
};

#endif // MINE_H