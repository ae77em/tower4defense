#ifndef TP4_TOWERDEFENSE_ANIMABLES_H
#define TP4_TOWERDEFENSE_ANIMABLES_H

#include <SDL2/SDL_rect.h>
#include "Point.h"

class Animable {
public:
    Animable() { }
    virtual ~Animable() { }
    virtual void animate(SDL_Rect &camera) = 0;
    virtual Point getPoint() = 0;
};


#endif //TP4_TOWERDEFENSE_ANIMABLES_H
