#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Tank.h"
#include "Mine.h"
#include "Bullet.h"
#include "Menu.h"

class CollisionHandler
{
public:
    static void handlePlayerMineCollisions(std::vector<std::unique_ptr<Mine>>& mines, std::unique_ptr<Tank>& player, int& playerLives);
    static void handleBulletMineCollisions(std::vector<std::unique_ptr<Mine>>& mines, std::unique_ptr<Bullet>& bullet);

};

