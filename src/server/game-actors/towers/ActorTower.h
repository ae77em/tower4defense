#ifndef TP4_TOWERDEFENSE_ACTORTOWER_H
#define TP4_TOWERDEFENSE_ACTORTOWER_H

#include "../../../common/Circle.h"
#include "../../../common/Point.h"
#include "../../GameActor.h"

class ActorTower {
public:
    ActorTower();

    std::string getClass();

    void live();

    int getDirection();

    int getXPosition();

    int getYPosition();

    int getEnergy();

    int getId();

    void shiftColliders();

    Circle &getCollisionCircle();

    int getShotDamage();

    void setIsShooting(bool isShooting);

    void sumExperiencePoints(int points);

    int getExperiencePoints();

protected:
    int shotRatio;
    int shotMsTimeGap;
    bool isShooting;
    int lastShotTime;
    int shotDamage;
    int experiencePoints;
    Circle collisionCircle;
    Point currentPoint = Point(0, 0);

    void initialize();
};


#endif //TP4_TOWERDEFENSE_ACTORTOWER_H
