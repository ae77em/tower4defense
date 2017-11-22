#include "ActorTowerEarth.h"
#include "../../../sdl/Constants.h"

ActorTowerEarth::ActorTowerEarth(int id) : ActorTower(id) {
    range = 2;
    reach = 0;
    shotDamage = 20;
    shotSecondsGap = 1;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerEarth::~ActorTowerEarth() { }
