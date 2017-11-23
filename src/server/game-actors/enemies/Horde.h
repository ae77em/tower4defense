#ifndef TP4_TOWERDEFENSE_SERVER_HORDE_H
#define TP4_TOWERDEFENSE_SERVER_HORDE_H

#include "ActorEnemy.h"
#include <vector>

class Horde {
public:
    Horde();

    Horde(int t);

    virtual ~Horde();

    void setEnemies(std::vector<ActorEnemy *> enemies);
    std::vector<ActorEnemy *> getEnemies();
    void addEnemy(ActorEnemy *enemy);

    static Horde *createHorde(int enemyType, int amount, std::vector<Point> path);

private:
    std::vector<ActorEnemy*> enemies;

    int type;

    static ActorEnemy *getEnemyByType(int type);
};


#endif //TP4_TOWERDEFENSE_SERVER_HORDE_H
