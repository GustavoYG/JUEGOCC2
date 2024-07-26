#include "CollisionHandler.h"

void CollisionHandler::handlePlayerMineCollisions(std::vector<std::unique_ptr<Mine>>& mines, std::unique_ptr<Tank>& player, int& playerLives) {
    for (auto it = mines.begin(); it != mines.end(); ) {
        if ((*it)->isActive()) {
            if ((*it)->getShape().getGlobalBounds().intersects(player->getShape().getGlobalBounds())) {
                playImpact();
                playerLives -= 2;
                it = mines.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}

void CollisionHandler::handleBulletMineCollisions(std::vector<std::unique_ptr<Mine>>& mines, std::unique_ptr<Bullet>& bullet) {
    for (auto it = mines.begin(); it != mines.end(); ) {
        if ((*it)->isActive() && bullet) {
            if ((*it)->getShape().getGlobalBounds().intersects(bullet->getShape().getGlobalBounds())) {
                playImpact();
                bullet.reset();
                it = mines.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}