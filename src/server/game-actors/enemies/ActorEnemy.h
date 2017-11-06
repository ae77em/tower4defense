#ifndef TP4_TOWERDEFENSE_SERVER_ENEMY_H
#define TP4_TOWERDEFENSE_SERVER_ENEMY_H

#include <vector>
#include "../../../common/Point.h"

class ActorEnemy {
private:
    std::vector<Point> path;
    int currentDirection;
    int currentPathPosition;
    int currentShift;
    Point currentPoint = Point(-1, -1);
    int xPosition;
    int yPosition;
    bool isWalking;

public:
    ActorEnemy();

    virtual ~ActorEnemy();

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
};

#endif //TP4_TOWERDEFENSE_SERVER_ENEMY_H
