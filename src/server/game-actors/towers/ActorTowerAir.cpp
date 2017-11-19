#include "ActorTowerAir.h"
#include "../../../sdl/Constants.h"

void ActorTowerAir::initialize() {
    shotRatio = 5;
    shotDamage = 2;
    shotMsTimeGap = 5000; //miliseconds
    slowDownPercentaje = 0.0;
    collisionCircle.r = (CARTESIAN_TILE_WIDTH / 2) * shotRatio;
}

int ActorTowerAir::getShotDamage(ActorEnemy enemy) {
    if (enemy.itIsAir()){
        return 10 + 10 * levelDamage;
    } else {
        return 2 + 2 * levelDamage;
    }
}


