#include "ActorTowerWater.h"
#include "../../../sdl/Constants.h"

ActorTowerWater::ActorTowerWater(int id) : ActorTower(id) {
    initialize();
}

void ActorTowerWater::initialize() {
    range = 3;
    shotDamage = 6;
    shotMsTimeGap = 3000; //miliseconds
    slowDownPercentaje = 0.0;
    collisionCircle.r = (CARTESIAN_TILE_WIDTH / 2) * range;
}
