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

    int getRange();

    int getReach();

    /* Para retornar info al juego de la torre */
    std::string getRangeInfo();

    std::string getReachInfo();

    std::string getSlowDownPercentajeInfo();

    std::string getShotDamageInfo();

protected:
    int range = 0;
    int reach = 0;
    int shotMsTimeGap = 0;
    bool isShooting = 0;
    int lastShotTime = 0;
    int shotDamage = 0;
    double slowDownPercentaje = 0.0;
    int experiencePoints = 0;


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
