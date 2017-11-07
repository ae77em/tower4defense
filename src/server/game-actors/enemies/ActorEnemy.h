#ifndef TP4_TOWERDEFENSE_SERVER_ENEMY_H
#define TP4_TOWERDEFENSE_SERVER_ENEMY_H

#include <vector>
#include <string>
#include <bits/basic_string.h>
#include "../../../common/Point.h"
#include "../../GameActor.h"

class ActorEnemy : public GameActor {
private:
    std::vector<Point> path;
    int currentDirection;
    int currentPathPosition;
    int currentShift;
    Point currentPoint = Point(-1, -1);
    int xPosition;
    int yPosition;
    bool isWalking;

    int velocity = 1;
    int initialLifePoints = 200;
    int remainingLifePoints = 200;
    int isAir = false;
    int isAlive = true;


public:
    ActorEnemy();

    void live();

    const Point &getCurrentPoint() const;

    void setCurrentPoint(const Point &currentPoint);

    int getCurrentPosition() const;

    void setCurrentPathPosition(int currentPosition);

    const std::vector<Point> &getPath() const;

    void setPath(const std::vector<Point> &path);

    int getCurrentDirection() const;

    void setCurrentDirection(int currentDirection);

    void advance();

    void gotoNextPathPosition();

    int getXPosition() const;

    int getYPosition() const;

    void setIsWalking(bool isWalking);

    int getEnergy();

    std::string getClass();
};

#endif //TP4_TOWERDEFENSE_SERVER_ENEMY_H
