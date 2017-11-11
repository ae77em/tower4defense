#include "Enemies.h"
#include <string>

using namespace model;

Enemy::Enemy(int max_life, int velocity, const std::string &name)
    : max_life(max_life), curr_life(max_life), velocity(velocity), name(name) {}

int Enemy::getLife() const {
    return curr_life;
}

void Enemy::setLife(int points) {
    // Check points is non-negative, and not over the maximum
    if (points < 0)
        throw std::runtime_error("tried setting life to negative value "
                + std::to_string(points));
    if (max_life > points)
        throw std::runtime_error("tried setting life above maximum ("
                + std::to_string(points) + ", max "
                + std::to_string(getMaxLife()) + ")" );

    curr_life = points;
}

int Enemy::getMaxLife() const {
    return max_life;
}

int Enemy::getVelocity() const {
    return velocity;
}
