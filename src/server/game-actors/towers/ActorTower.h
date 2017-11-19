#ifndef TP4_TOWERDEFENSE_ACTORTOWER_H
#define TP4_TOWERDEFENSE_ACTORTOWER_H

#include "../../../common/Circle.h"
#include "../../../common/Point.h"
#include "../../GameActor.h"
#include "../enemies/Horde.h"
#include "../enemies/ActorEnemy.h"

#include <string>

class ActorTower  {
public:
    ActorTower();

    virtual ~ActorTower();

    std::string getClass();

    void live();

    void live(Horde *horde);

    int getXPosition();

    int getYPosition();

    int getEnergy();

    int getId();

    ActorRectT getRect();

    void shiftColliders();

    Circle &getCollisionCircle();

    int getShotDamage();

    void setIsShooting(bool isShooting);

    void sumExperiencePoints(int points);

    int getExperiencePoints();

    bool itIsShooting();

    void setPosition(int x, int y);

protected:
    int shotRatio;
    int shotMsTimeGap;
    bool isShooting;
    int lastShotTime;
    int shotDamage;
    int experiencePoints;
    Circle collisionCircle;
    Point currentPoint = Point(0, 0);
    ActorRectT rect;

    void initialize();

    void shootTo(ActorEnemy *pEnemy);

    void doShootTo(ActorEnemy *pEnemy);
};


#endif //TP4_TOWERDEFENSE_ACTORTOWER_H
