#ifndef TP4_TOWERDEFENSE_DrawableHorde_H
#define TP4_TOWERDEFENSE_DrawableHorde_H

#include "Enemy.h"
#include <vector>

class DrawableHorde {
private:
    std::vector<Enemy*> enemies;
    int enemySeparation; // in pixels, in cartesian coords

public:
    DrawableHorde();
    DrawableHorde(int enemyType, size_t size, int x, int y,
                  SDL_Renderer *renderer, Texture &texture);
    virtual ~DrawableHorde();

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


#endif //TP4_TOWERDEFENSE_DrawableHorde_H
