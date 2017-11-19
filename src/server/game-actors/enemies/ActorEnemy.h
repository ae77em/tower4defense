#ifndef TP4_TOWERDEFENSE_SERVER_ENEMY_H
#define TP4_TOWERDEFENSE_SERVER_ENEMY_H

#include <vector>
#include <string>
#include <bits/basic_string.h>
#include "../../../common/Point.h"
#include "../../../common/Circle.h"
#include "../../GameActor.h"

class ActorEnemy : public GameActor {
public:
    ActorEnemy();

    virtual ~ActorEnemy();

    std::string getClass();

    void live() override;

    int getDirection() override;

    int getXPosition() override;

    int getYPosition() override;

    int getEnergy() override;

    int getId() override;

    ActorRectT getRect() override;

    const Point &getCurrentPoint() const;

    void setCurrentPoint(const Point &currentPoint);

    int getCurrentPosition() const;

    void setCurrentPathPosition(int currentPosition);

    const std::vector<Point> &getPath() const;

    void setPath(const std::vector<Point> &path);

    void setDirection(int currentDirection);

    void advance();

    void gotoNextPathPosition();

    void setIsWalking(bool isWalking);

    void setId(int id);

    int getXPositionIntoTile();

    int getYPositionIntoTile();

    Circle &getCollisionCircle();

    bool itIsAlive();

    bool itIsAir();

    int getShoot(int i);

    int receiveDamage(int i);

protected:
    std::vector<Point> path;
    Point currentPoint = Point(-1, -1);
    Circle collisionCircle;

    int xPositionIntoTile = 0;
    int currentDirection = 0;
    int currentPathPosition = 0;
    int currentShift = 0;
    int yPositionIntoTile = 0;
    int xPosition = 0;
    int yPosition = 0;
    bool isWalking = false;

    int velocity = 1;
    int initialEnergy = 200;
    int energy = 200;
    int remainingLifePoints = 200;
    bool isAir = false;
    bool isAlive = true;


    int id;
};

#endif //TP4_TOWERDEFENSE_SERVER_ENEMY_H
