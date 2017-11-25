#include "ActorEnemy.h"
#include "../../../common/Utils.h"
#include "../../../sdl/Constants.h"
#include "../../../common/Circle.h"
#include <string>
#include <vector>

ActorEnemy::ActorEnemy() {
    collisionCircle.r = CARTESIAN_TILE_HEIGHT * 0.5;
}

ActorEnemy::~ActorEnemy() {}

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

int ActorEnemy::getCurrentPathPosition() const {
    return currentPathPosition;
}

void ActorEnemy::setCurrentPathPosition(int currPos) {
    currentPathPosition = currPos;
}

void ActorEnemy::advance() {
    if (currentPathPosition < 0) {
        currentPathPosition += getCalculatedVelocity();
        isVisible = false;
    } else if (isOnStage()) {
        if (!isVisible){ // recién entro al escenario...
            currentPathPosition = 0;
            isVisible = true;
        }

        if (xPositionIntoTile > CARTESIAN_TILE_WIDTH ||
            yPositionIntoTile > CARTESIAN_TILE_HEIGHT) {
            ++currentPathPosition;
            xPositionIntoTile = 0;
            yPositionIntoTile = 0;
            xPositionIntoTileFraction = 0.0;
            yPositionIntoTileFraction = 0.0;
        }

        currentPoint = path.at(currentPathPosition);

        if ((unsigned) currentPathPosition < path.size() - 1) {
            Point point = path.at(currentPathPosition + 1);
            xFinalIntoTile = point.x;
            yFinalIntoTile = point.y;
        }

        int xMovement = Utils::getNextMapDisplacement(currentPoint.x,
                                                      xFinalIntoTile);
        int yMovement = Utils::getNextMapDisplacement(currentPoint.y,
                                                      yFinalIntoTile);

        // El enemigo tiene movimiento para algún lugar, lo muevo.
        if (xMovement > 0 || yMovement > 0) {
            int x = path.at(currentPathPosition).x + xMovement;
            int y = path.at(currentPathPosition).y + yMovement;

            currentDirection =
                    Utils::getMovementDirection(
                            Utils::getNextMapDisplacement(x, xFinalIntoTile),
                            Utils::getNextMapDisplacement(y, yFinalIntoTile));

            /* Calculo siguiente posición con la velocidad. Uso valores
             * flotantes justamente para no perder los avances medios, por
             * ejemplo, si avanzo de a 1,5 pixeles, voy a dibujar un avance
             * de 1 pixel, pero el próximo avance será la posición 3. Para
             * salvar esos valores intermedios necesito un valor de punto
             * flotante.
             * */
            xPositionIntoTileFraction +=
                    xMovement * getCalculatedVelocity();
            yPositionIntoTileFraction +=
                    yMovement * getCalculatedVelocity();

            /* La posición dentro del que le voy a pasar al cliente para que
             * dibuje debe ser entera, así que tomo la parte entera del
             * fraccionario anterior.
             * */
            xPositionIntoTile = (int)xPositionIntoTileFraction;
            yPositionIntoTile = (int)yPositionIntoTileFraction;

            /* Calculo la posición dentro del mapa.
             * */
            xPosition = path.at(currentPathPosition).x + xPositionIntoTile;
            yPosition = path.at(currentPathPosition).y + yPositionIntoTile;

            /* Calculo el círculo de colisión.
             * */
            collisionCircle.x = xPosition + int(CARTESIAN_TILE_WIDTH * 0.5);
            collisionCircle.y = yPosition + int(CARTESIAN_TILE_HEIGHT * 0.5);
        /* Final del camino, señalar final de juego */
        } else {
            isVisible = false;
            endedThePath = true;
        }
    }
}

bool ActorEnemy::isOnStage() const {
    return currentPathPosition >= 0 && currentPathPosition < (int)path.size();
}

double ActorEnemy::getCalculatedVelocity() const {
    return velocity * (1.0 - slowdown) * VEL_REGULATOR;
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
    return energy;
}

double ActorEnemy::getRemainingEnergyPercentaje() {
    return double(energy) / double(initialEnergy);
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

bool ActorEnemy::getIsAlive() {
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

    if (energy > 0) {
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

bool ActorEnemy::hasEndedThePath() {
    return endedThePath;
}

