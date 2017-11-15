#ifndef TP4_TOWERDEFENSE_ABMONIBLE_H
#define TP4_TOWERDEFENSE_ABMONIBLE_H


#include "Enemy.h"

class Abmonible : public Enemy {
public:
    Abmonible(int x, int y, SDL_Renderer *renderer, Texture *texture);

};

#endif //TP4_TOWERDEFENSE_ABMONIBLE_H
