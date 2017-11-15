#ifndef TP4_TOWERDEFENSE_SERVER_HORDE_H
#define TP4_TOWERDEFENSE_SERVER_HORDE_H

#include "ActorEnemy.h"

class Horde {
public:
    Horde();
    virtual ~Horde();

    void setEnemies(std::vector<ActorEnemy *> enemies);
    std::vector<ActorEnemy *> getEnemies();
    void addEnemy(ActorEnemy *enemy);

private:
    std::vector<ActorEnemy*> enemies;
};


#endif //TP4_TOWERDEFENSE_SERVER_HORDE_H
