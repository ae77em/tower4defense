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

    ActorTower(int id);

    virtual ~ActorTower();

    std::string getClass();

    void live();

    void attack(Horde *horde);

    int getXPosition();

    int getYPosition();

    int getEnergy();

    int getId();

    ActorRectT getRect();

    void shiftColliders();

    Circle &getCollisionCircle();

    virtual int getShotDamage(ActorEnemy enemy);

    void setIsShooting(bool isShooting);

    void sumExperiencePoints(int points);

    int getExperiencePoints();

    int getSlowDownPercentaje();

    void setSlowDownPercentaje(double perc);

    bool itIsShooting();

    void setPosition(int x, int y);

    virtual void initialize();

    void shootTo(ActorEnemy *pEnemy);

    void doShootTo(ActorEnemy *pEnemy);

protected:
    int shotRatio;
    int shotMsTimeGap;
    bool isShooting;
    int lastShotTime;
    int shotDamage;
    double slowDownPercentaje;
    int experiencePoints;


    int levelRange = 0;
    int levelDamage = 0;
    int levelReach = 0;
    int levelSlowdown = 0;

    Circle collisionCircle;
    Point currentPoint = Point(0, 0);
    ActorRectT rect;

    int id;
};


#endif //TP4_TOWERDEFENSE_ACTORTOWER_H
