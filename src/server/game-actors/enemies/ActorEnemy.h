#ifndef TP4_TOWERDEFENSE_SERVER_ENEMY_H
#define TP4_TOWERDEFENSE_SERVER_ENEMY_H

#include <vector>
#include <string>
#include <bits/basic_string.h>
#include "../../../common/Point.h"
#include "../../GameActor.h"

class ActorEnemy : public GameActor {
public:
    ActorEnemy();

    std::string getClass();

    void live();

    int getDirection();

    int getXPosition();

    int getYPosition();

    int getEnergy();

    int getId();

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

private:
    std::vector<Point> path;
    int currentDirection = 0;
    int currentPathPosition = 0;
    int currentShift = 0;
    Point currentPoint = Point(-1, -1);
    int xPositionIntoTile = 0;
    int yPositionIntoTile = 0;
    int xPosition = 0;
    int yPosition = 0;
    bool isWalking = false;

    int velocity = 1;
    int initialLifePoints = 200;
    int remainingLifePoints = 200;
    int isAir = false;
    int isAlive = true;

    int id;
};

#endif //TP4_TOWERDEFENSE_SERVER_ENEMY_H
