#include "ActorTowerWater.h"
#include "../../../sdl/Constants.h"

ActorTowerWater::ActorTowerWater(int id) : ActorTower(id) {
    range = 3;
    reach = 0;
    shotDamage = 4;
    shotMsTimeGap = 3;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.25;
    experiencePoints = 0;
}

ActorTowerWater::~ActorTowerWater() { }
