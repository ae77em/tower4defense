#include "ActorTowerFire.h"
#include "../../../sdl/Constants.h"

void ActorTowerFire::initialize() {
    shotRatio = 3;
    shotDamage = 6;
    shotMsTimeGap = 3000; //miliseconds
    slowDownPercentaje = 0.0;
    collisionCircle.r = (CARTESIAN_TILE_WIDTH / 2) * shotRatio;
}

ActorTowerFire::ActorTowerFire(int id) : ActorTower(id) { }
