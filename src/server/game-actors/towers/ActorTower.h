#ifndef TP4_TOWERDEFENSE_ACTORTOWER_H
#define TP4_TOWERDEFENSE_ACTORTOWER_H

#include "Circle.h"
#include "Point.h"
#include "GameActor.h"
#include "ActorHorde.h"
#include "ActorEnemy.h"

#include <string>

class ActorTower  {
public:
    ActorTower();

    explicit ActorTower(int id);

    virtual ~ActorTower();

    virtual std::string getClass();

    void live();

    void attack(ActorHorde *horde);

    int getId();

    void shiftColliders();

    Circle &getCollisionCircle();

    virtual int getShotDamage(ActorEnemy *enemy);

    void setIsShooting(bool isShooting);

    void sumExperiencePoints(int points);

    bool itIsShooting();

    void setPosition(int x, int y);

    void shootTo(ActorEnemy *pEnemy);

    void doShootTo(ActorEnemy *pEnemy);

    /* Para retornar info al juego de la torre */
    virtual std::string getRangeInfo();

    virtual std::string getReachInfo();

    virtual std::string getSlowDownPercentajeInfo();

    virtual std::string getShotDamageInfo();

    bool isReadyToShoot() const;

    virtual bool upgradeDamage();

    virtual bool upgradeRange();

    virtual bool upgradeSlowdown();

    virtual bool upgradeReach();

    std::string getExperiencePointsInfo();

    void setId(int aId);

    std::string getClassInfo();

protected:
    int range;
    int reach;
    int shotSecondsGap;
    bool isShooting;
    int lastShotTime;
    int shotDamage;
    int reachDamage;
    float slowdownPercentaje;
    int slowdownDuration;
    int experiencePoints;

    int rangeLevel;
    int shotDamageLevel;
    int reachLevel;
    int slowdownLevel;

    Circle collisionCircle;
    Point currentPoint = Point(0, 0);

    int id;

    void updateLastShotTime();

    int getCollisionCircleRadio();

    virtual void initialize();

    virtual void doAttack(ActorHorde *horde);
};


#endif //TP4_TOWERDEFENSE_ACTORTOWER_H
