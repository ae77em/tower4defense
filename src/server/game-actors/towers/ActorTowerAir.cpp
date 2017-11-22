#include "ActorTowerAir.h"
#include "../../../sdl/Constants.h"

ActorTowerAir::ActorTowerAir(int id) : ActorTower(id) {
    range = 5;
    reach = 0;
    shotDamage = 2;
    shotMsTimeGap = 5;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerAir::~ActorTowerAir() { }

int ActorTowerAir::getShotDamage(ActorEnemy enemy) {
    if (enemy.itIsAir()){
        return 10 + 10 * levelDamage;
    } else {
        return 2 + 2 * levelDamage;
    }
}
