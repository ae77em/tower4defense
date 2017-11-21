#include "ActorTowerEarth.h"
#include "../../../sdl/Constants.h"

ActorTowerEarth::ActorTowerEarth(int id) : ActorTower(id) { }

void ActorTowerEarth::initialize() {
    range = 2;
    shotDamage = 20;
    shotMsTimeGap = 3000; //miliseconds
    slowDownPercentaje = 0.0;
    collisionCircle.r = (CARTESIAN_TILE_WIDTH / 2) * range;
}
