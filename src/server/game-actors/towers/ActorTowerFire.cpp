#include "ActorTowerFire.h"
#include "../../../sdl/Constants.h"

ActorTowerFire::ActorTowerFire(int id) : ActorTower(id) {
    initialize();
}

void ActorTowerFire::initialize() {
    range = 3;
    reach = 1;
    shotDamage = 6;
    shotMsTimeGap = 3000; //miliseconds
    slowDownPercentaje = 0.0;
    collisionCircle.r = (CARTESIAN_TILE_WIDTH / 2) * range;
}

