#include "WorkerLoopGame.h"
#include "../common/Point.h"
#include "../sdl/Constants.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "../common/MessageFactory.h"

#include <iostream>
#include <chrono>
#include <thread>


//HAY QUE AGREGAR MUTEX PLAYERD PARA QUE BLOQUEE LA LISTA DE SERVER PLAYERS
//PUEDE SER QUE SE ESTE REMOVIENDO ALGUNO EN OTRO HILO Y LUEGO EXPLOTE
WorkerLoopGame::WorkerLoopGame(std::vector<ServerPlayer*>& p,
                               std::list<GameAction*>& a,
                               std::mutex& m):
                                                players(p),
                                                actions(a),
                                                mutexActions(m){
}

void WorkerLoopGame::run(){
    std::cout << "WorkerLoopGame: Hilo donde existe la partida arrancando" << std::endl;
    unsigned int ciclos = 1000;
    std::string statusGame;

    //while(isRunning()){
    while (ciclos > 0){
        //std::list<GameAction*> obtainedActions = getActions();
        std::list<GameAction *> actionsGame;

        mutexActions.lock();
        for (GameAction* a : this->actions) {
            actionsGame.push_back(a);
        }
        mutexActions.unlock();

        //TOMO LAS ACCIONES PARSEADAS DE LOS REQUEST Y LAS APLICO A TODOS LOS ACTORES
        /*for (GameAction* a : actionsGame) {
            for (GameActor* g : gameActors){

            }
        }*/

        //luego de modificar el estado de cada actor hago que vivan
        /*for (GameActor* g : gameActors) {
            g->live();
        }*/

        for (std::map<int, std::vector<ActorEnemy*>>::iterator it = hordas.begin(); it != hordas.end(); ++it){
            std::vector<ActorEnemy*> enemies = it->second;

            for (auto enemy : enemies) {
                enemy->live();
            }
        }

        //GET STATUS GAMES
        statusGame = getGameStatus();

        //NOTIFICO EL ESTADO DEL JUEGO A TODOS LO JUGADORES LUEGO DE LA MODIFICACION DE MODELO
        for(ServerPlayer* s : players){
            s->sendData(statusGame);
        }

        ciclos--;

        //std::this_thread::sleep_for(std::chrono::seconds(0.5));
    }

    std::cout << "WorkerLoopGame: Hilo donde existe la partida se termino" << std::endl;
    std::cout << "WorkerLoopGame: Aviso que terminó la partida." << std::endl;
    statusGame = MessageFactory::getMatchEndedNotification();
    for(ServerPlayer* s : players){
        s->sendData(statusGame);
    }
}

void WorkerLoopGame::buildGameContext() {
    std::vector<Point> camino1;
    // supongo que recorro una matriz de baldosas de CARTESIAN_TILE_WIDTH x CARTESIAN_TILE_HEIGHT
    camino1.push_back(Point(0 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(1 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(2 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(3 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(4 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(4 * CARTESIAN_TILE_WIDTH,6 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(4 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(5 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(6 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(7 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(8 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(9 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(10 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(11 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(12 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(12 * CARTESIAN_TILE_WIDTH,6 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(12 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(13 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(14 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(15 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino1.push_back(Point(16 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));

    std::vector<ActorEnemy*> horda1;

    for (int x = 0; x < 3; ++x){
        ActorEnemy* enemy = new ActorEnemy();
        enemy->setPath(camino1);
        enemy->setId(x);
        enemy->setCurrentPathPosition(-x * CARTESIAN_TILE_WIDTH / 2);

        horda1.push_back( enemy );
    }
    hordas.insert(std::pair<int, std::vector<ActorEnemy*>>(0, horda1));

    std::vector<Point> camino2;
    // supongo que recorro una matriz de baldosas de CARTESIAN_TILE_WIDTH x CARTESIAN_TILE_HEIGHT
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 0 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 1 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 2 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 3 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 4 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 5 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 6 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 7 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 8 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 9 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 10 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 11 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 12 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 13 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 14 * CARTESIAN_TILE_HEIGHT));
    camino2.push_back(Point(7 * CARTESIAN_TILE_WIDTH, 15 * CARTESIAN_TILE_HEIGHT));


    std::vector<ActorEnemy*> horda2;

    for (int x = 0; x < 3; ++x){
        ActorEnemy* enemy = new ActorEnemy();
        enemy->setPath(camino2);
        enemy->setId(x);
        enemy->setCurrentPathPosition(-x * CARTESIAN_TILE_WIDTH / 2);

        horda2.push_back( enemy );
    }
    hordas.insert(std::pair<int, std::vector<ActorEnemy*>>(1, horda2));


    /* ESTA ES LÓGICA DE NEGOCIO, Y VA EN EL SERVER...
     * bool towerIsShooting =
            enemy->itIsAlive() &&
            enemy->getCollisionCircle().hasCollisionWith(tower.getCollisionCircle());
    if (towerIsShooting) {
        tower.setIsShooting(true);
        int shotDamage = tower.getShotDamage();
        enemy->quitLifePoints(shotDamage);
        tower.sumExperiencePoints(shotDamage);
        std::cout << "la torre tiene " << tower.getExperiencePoints() << " de experiencia."
                  << std::endl;
    } else {
        tower.setIsShooting(false);
    }

    if (enemy->itIsAlive()) {
        enemy->setDirection(dir);
        enemy->moveTo(point.x, point.y);
    } else if (towerIsShooting) {
        tower.sumExperiencePoints(enemy->getBonus());
        std::cout << "la torre sumó bonus de " << enemy->getBonus() << " puntos de experiencia."
                  << std::endl;

        std::vector<Enemy*> enemies = static_cast<std::vector<Enemy *> &&>(horde->getEnemies());
        for (Enemy *enemy1 : enemies) {
            if (enemy1->itIsAlive()) {
                gameWon = false;
                break;
            } else {
                gameWon = true;
            }
        }
    }

    if (enemy->itIsAlive()) {
        if (point.x >= finalPoint.x && point.y >= finalPoint.y) {
            gameLoose = true;
        }
    }

    std::cout << "la torre tiene " << tower.getExperiencePoints() << " de experiencia." << std::endl;
     */
}

std::string WorkerLoopGame::getGameStatus() {
    //despues ver como parsear todos los actores
    return MessageFactory::getStatusMatchNotification(hordas);
}
