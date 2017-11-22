#include "ActorTowerFire.h"
#include "../../../sdl/Constants.h"

ActorTowerFire::ActorTowerFire(int id) : ActorTower(id) {
    range = 3;
    reach = 1;
    shotDamage = 6;
    shotMsTimeGap = 3;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerFire::~ActorTowerFire() { }
