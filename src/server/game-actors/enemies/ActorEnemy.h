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

    void setCurrentPathPosition(int currentPosition);

    void setPath(const std::vector<Point> &path);

    void advance();

    void setId(int id);

    Circle &getCollisionCircle();

    bool itIsAlive();

    bool itIsAir();

    bool itIsVisible();

    int receiveDamage(int i);

    bool hasEndedThePath();

    double getRemainingEnergyPercentaje();

    void setSlowdown(double slowdown, int timeOfSlowdown);

protected:
    /* Datos básicos de los enemigos.
     * Podrían ser tomados de un archivo de configuración.
     * */
    int velocity = 1;
    int initialEnergy = 200;
    int energy = 200;
    bool isAir = false;
    /********************/

    int id;
    std::vector<Point> path;
    Point currentPoint = Point(-1, -1);
    Circle collisionCircle;

    const double VEL_REGULATOR = 0.25; // para que los bichos no vayan
    // demasiado rápido o lento...

    double positionIntoTileFraction = 0.0;
    double xPositionIntoTileFraction = 0.0;
    double yPositionIntoTileFraction = 0.0;
    int xPositionIntoTile = 0;
    int yPositionIntoTile = 0;
    int currentDirection = 0;
    /* Cuanto camino ha recorrido el enemigo
     * Para valores negativos de esta variable, el enemigo no esta
     * en pantalla y es invisible.
     */
    int currentPathPosition = 0;
    int xPosition = 0;
    int yPosition = 0;
    bool isVisible = false;
    int xFinalIntoTile = -1;
    int yFinalIntoTile = -1;
    bool isAlive = true;
    bool endedThePath = false;
    double slowdown = 0.0; // varía entre 0 y 1
    time_t timeOfLastSlowdown = 0;
    time_t timeOfSlowdown = 0;

    double getCalculatedVelocity();

    bool isOnStage() const;

};

#endif //TP4_TOWERDEFENSE_SERVER_ENEMY_H
