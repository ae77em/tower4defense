#include "ActorEnemy.h"
#include "../../../common/Utils.h"


ActorEnemy::ActorEnemy() {}

const std::vector<Point> &ActorEnemy::getPath() const {
    return path;
}

void ActorEnemy::setPath(const std::vector<Point> &path) {
    ActorEnemy::path = path;
}

int ActorEnemy::getDirection() {
    return currentDirection;
}

void ActorEnemy::setDirection(int currentDirection) {
    ActorEnemy::currentDirection = currentDirection;
}

int ActorEnemy::getCurrentPosition() const {
    return currentPathPosition;
}

void ActorEnemy::setCurrentPathPosition(int currentPosition) {
    ActorEnemy::currentPathPosition = currentPosition;
}

void ActorEnemy::advance() {
    if (currentPathPosition >= 0) {
        int xFinal, yFinal;

        currentPoint = path.at(currentPathPosition);

        if ((unsigned) currentPathPosition < path.size() - 1) {
            Point point = path.at(currentPathPosition + 1);
            xFinal = point.x;
            yFinal = point.y;
        }

        int xMovement = Utils::getNextMapDisplacement(currentPoint.x, xFinal);
        int yMovement = Utils::getNextMapDisplacement(currentPoint.y, yFinal);

        int x = path.at(currentPathPosition).x + xMovement;
        int y = path.at(currentPathPosition).y + yMovement;

        currentDirection = Utils::getMovementDirection(Utils::getNextMapDisplacement(x, xFinal),
                                                       Utils::getNextMapDisplacement(y, yFinal));

        xPosition += xMovement;
        yPosition += yMovement;
    } else {
        ++currentPathPosition;
    }
}

void ActorEnemy::gotoNextPathPosition() {
    ++currentPathPosition;
}

void ActorEnemy::setIsWalking(bool iw) {
    if (!isWalking) {
        isWalking = iw;
        currentPathPosition = 0;
        currentPoint = path.at(currentPathPosition);
        xPosition = currentPoint.x;
        yPosition = currentPoint.y;
    }
}

const Point &ActorEnemy::getCurrentPoint() const {
    return currentPoint;
}

void ActorEnemy::setCurrentPoint(const Point &currentPoint) {
    ActorEnemy::currentPoint = currentPoint;
}

int ActorEnemy::getXPosition() {
    return xPosition;
}

int ActorEnemy::getYPosition() {
    return yPosition;
}

void ActorEnemy::live() {
    advance();
}

int ActorEnemy::getEnergy() {
    return remainingLifePoints;
}

std::string ActorEnemy::getClass() {
    return "ActorEnemy";
}

int ActorEnemy::getId() {
    return id;
}

void ActorEnemy::setId(int id) {
    ActorEnemy::id = id;
}
