#include "ActorEnemy.h"
#include "../../../common/Utils.h"
#include "../../../sdl/Constants.h"
#include "../../../common/Circle.h"
#include <string>
#include <vector>

ActorEnemy::ActorEnemy(){ }

ActorEnemy::~ActorEnemy(){ }

const std::vector<Point> &ActorEnemy::getPath() const {
    return path;
}

void ActorEnemy::setPath(const std::vector<Point> &path) {
    ActorEnemy::path = path;
}

int ActorEnemy::getDirection() {
    return currentDirection;
}

void ActorEnemy::setDirection(int currDir) {
    currentDirection = currDir;
}

int ActorEnemy::getCurrentPosition() const {
    return currentPathPosition;
}

void ActorEnemy::setCurrentPathPosition(int currPos) {
    currentPathPosition = currPos;
}

void ActorEnemy::advance() {
    if (currentPathPosition >= 0 && currentPathPosition < (int)path.size()) {
        int xFinal, yFinal;

        if (xPositionIntoTile > CARTESIAN_TILE_WIDTH ||
            yPositionIntoTile > CARTESIAN_TILE_HEIGHT){
            ++currentPathPosition;
            xPositionIntoTile = 0;
            yPositionIntoTile = 0;
        }

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

        currentDirection =
                Utils::getMovementDirection(
                        Utils::getNextMapDisplacement(x, xFinal),
                        Utils::getNextMapDisplacement(y, yFinal));

        xPositionIntoTile += xMovement;
        yPositionIntoTile += yMovement;

        xPosition = path.at(currentPathPosition).x + xPositionIntoTile;
        yPosition = path.at(currentPathPosition).y + yPositionIntoTile;

        collisionCircle.x = xPosition + (CARTESIAN_TILE_WIDTH * 0.5);
        collisionCircle.y = xPosition + (CARTESIAN_TILE_HEIGHT * 0.5);
        isVisible = true;
    } else if (currentPathPosition < (int)path.size()) {
        ++currentPathPosition;
        isVisible = false;
    } else {
        isVisible = false;
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
        xPositionIntoTile = currentPoint.x;
        yPositionIntoTile = currentPoint.y;
    }
}

const Point &ActorEnemy::getCurrentPoint() const {
    return currentPoint;
}

void ActorEnemy::setCurrentPoint(const Point &currentPoint) {
    ActorEnemy::currentPoint = currentPoint;
}


int ActorEnemy::getXPositionIntoTile() {
    return xPositionIntoTile;
}

int ActorEnemy::getYPositionIntoTile() {
    return yPositionIntoTile;
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

Circle &ActorEnemy::getCollisionCircle() {
    return collisionCircle;
}

bool ActorEnemy::itIsAlive() {
    return isAlive;
}

int ActorEnemy::getShoot(int damage) {
    int actualDamage = std::min(damage, energy);
    energy -= actualDamage;

    return actualDamage;
}

int ActorEnemy::receiveDamage(int damage) {
    int damagePoints = 0;
    int actualDamage = std::min(damage, energy);
    energy -= actualDamage;

    if (energy > 0){
        damagePoints = actualDamage;
    } else {
        int bonus = initialEnergy * 0.5;
        isAlive = false;
        damagePoints += actualDamage + bonus;
    }

    return damagePoints;
}

ActorRectT ActorEnemy::getRect() {
    return ActorRectT();
}

bool ActorEnemy::itIsAir() {
    return isAir;
}

