#include "DrawableHorde.h"
#include "Abmonible.h"

DrawableHorde::DrawableHorde() {}

DrawableHorde::DrawableHorde(int enemyType, size_t size, int x, int y, SDL_Renderer *r, Texture &t) {
    switch (enemyType){
        case ABMONIBLE:{
            loadAbmonibles(size, x, y, r);
            break;
        }
        default:{
            //do nothing;
        }
    }
}

DrawableHorde::~DrawableHorde() {}

void DrawableHorde::loadAbmonibles(size_t size, int x, int y, SDL_Renderer *r) {
    enemySeparation = Abmonible::getCollisionCircleRadio();
    for (unsigned i = 0; i < size; ++i){
        enemies.push_back(new Abmonible(x, y, r, nullptr));
    }
}

void DrawableHorde::loadBloodHawks(size_t size) {

}

void DrawableHorde::loadGoatmans(size_t size) {

}

void DrawableHorde::loadGreenDaemons(size_t size) {

}

void DrawableHorde::loadSpectres(size_t size) {

}

void DrawableHorde::loadZombies(size_t size) {

}

void DrawableHorde::addEnemy(Enemy *e){
    enemies.push_back(e);
}

std::vector<Enemy *> &DrawableHorde::getEnemies() {
    return enemies;
}

Enemy* &DrawableHorde::getEnemieAt(int i) {
    return enemies.at((unsigned)i);
}
