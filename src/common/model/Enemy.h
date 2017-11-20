#ifndef ENEMIES_H
#define ENEMIES_H

#include "../Point.h"
#include <string>
#include <vector>
#include <functional>

namespace model {

/* Para mostrar una instancia de Enemy por pantalla, construir version
   visualizable a partir de enemy.getName() */
class Enemy {
    int max_life, curr_life;
    int velocity;
    const std::string &name; // eg: "abominable", not "fred"

    public:
    Enemy(int max_life, int velocity, const std::string &name);

    int getLife() const;
    void setLife(int points);

    int getMaxLife() const;
    int getVelocity() const;
};

} // namespace model

#endif
