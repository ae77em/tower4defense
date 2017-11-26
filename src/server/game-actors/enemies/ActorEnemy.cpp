#include "ActorEnemy.h"
#include "CommonUtils.h"
#include "Circle.h"
#include <string>
#include <vector>
#include <Protocol.h>

ActorEnemy::ActorEnemy() {
    collisionCircle.r = CARTESIAN_TILE_HEIGHT * 0.5;
    collisionCircle.x = 0;
    collisionCircle.y = 0;
}

ActorEnemy::~ActorEnemy() {}

void ActorEnemy::setPath(const std::vector<Point> &path) {
    ActorEnemy::path = path;
}

int ActorEnemy::getDirection() {
    return currentDirection;
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

        if (std::abs(xPositionIntoTile) > CARTESIAN_TILE_WIDTH ||
                std::abs(yPositionIntoTile) > CARTESIAN_TILE_HEIGHT) {
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

        int xMovement = CommonUtils::getNextMapDisplacement(currentPoint.x,
                                                      xFinalIntoTile);
        int yMovement = CommonUtils::getNextMapDisplacement(currentPoint.y,
                                                      yFinalIntoTile);

        // El enemigo tiene movimiento para algún lugar, lo muevo.
        if (xMovement != 0 || yMovement != 0) {
            int x = path.at(currentPathPosition).x + xMovement;
            int y = path.at(currentPathPosition).y + yMovement;

            currentDirection =
                    CommonUtils::getMovementDirection(
                            CommonUtils::getNextMapDisplacement(x, xFinalIntoTile),
                            CommonUtils::getNextMapDisplacement(y, yFinalIntoTile));

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

double ActorEnemy::getCalculatedVelocity() {
    time_t now;
    time(&now);

    if (now - timeOfLastSlowdown > timeOfSlowdown){
        slowdown = 0.0;
    }

    return velocity * (1.0 - slowdown) * VEL_REGULATOR;
}

void ActorEnemy::setSlowdown(double slowdown, int aTimeOfSlowdown){
    time_t now;
    time(&now);

    if (now - timeOfLastSlowdown > timeOfSlowdown){
        ActorEnemy::slowdown = slowdown;
        timeOfLastSlowdown = now;
        timeOfSlowdown = aTimeOfSlowdown;
    }
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

bool ActorEnemy::itIsAlive() {
    return isAlive;
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

bool ActorEnemy::itIsAir() {
    return isAir;
}

bool ActorEnemy::hasEndedThePath() {
    return endedThePath;
}

bool ActorEnemy::itIsVisible() {
    return isVisible;
}

