#ifndef TP4_TOWERDEFENSE_HORDE_H
#define TP4_TOWERDEFENSE_HORDE_H

#include "Enemy.h"

class Horde {
private:
    std::vector<Enemy*> enemies;
    int enemySeparation; // in pixels, in cartesian coords

public:
    Horde();
    Horde(int enemyType, size_t size, int x, int y, SDL_Renderer *renderer, LTexture &texture);
    virtual ~Horde();

    std::vector<Enemy *> &getEnemies();

    void addEnemy(Enemy *e);

    Enemy *&getEnemieAt(int i);

private:
    void loadAbmonibles(size_t size, int x, int y, SDL_Renderer *r);
    void loadBloodHawks(size_t size);
    void loadGoatmans(size_t size);
    void loadGreenDaemons(size_t size);
    void loadSpectres(size_t size);
    void loadZombies(size_t size);

};


#endif //TP4_TOWERDEFENSE_HORDE_H
