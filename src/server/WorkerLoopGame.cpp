#include "WorkerLoopGame.h"
#include "../common/Point.h"
#include "../sdl/Constants.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "game-actors/enemies/Horde.h"
#include "../common/MessageFactory.h"
#include "GameNotification.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <map>
#include <string>
#include <list>
#include <vector>

//HAY QUE AGREGAR MUTEX PLAYERD PARA QUE BLOQUEE LA LISTA DE SERVER PLAYERS
//PUEDE SER QUE SE ESTE REMOVIENDO ALGUNO EN OTRO HILO Y LUEGO EXPLOTE
WorkerLoopGame::WorkerLoopGame(std::map<int,ServerPlayer*>& p,
                               std::list<GameAction*>& a,
                               std::mutex& m):
                                                players(p),
                                                actions(a),
                                                mutexActions(m){
}

void WorkerLoopGame::run(){
    std::cout << "WorkerLoopGame: Hilo donde existe la partida arrancando" << std::endl;
    unsigned int ciclos = 10000;
    std::string statusGame;

    std::list<GameAction *> actionsGame;
    std::map<int, Horde*>::iterator hordeIt;

    bool clientDie = false;
    bool gameFinish = false;
    std::string cause = "";
    Point finalPoint(16 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT);

    //while(isRunning()){
    while (!gameFinish){
        std::cout << "WorkerLoopGame: ciclo  "<< ciclos << std::endl;

        //std::list<GameAction*> obtainedActions = getActions();

        mutexActions.lock();
        //GRONCHADA PARA REFACTORIZAR
        //IMPLEMENTAR CLASE QUE HAGA EL CIERRE DEL GAME LOOP

        /* Estos son los requests enviados por el cliente, por ejemplo,
         * poner torre... */
        for (GameAction* a : this->actions) {
            std::cout<< a->action << "  "<<std::endl;

            if(a->action.compare("game-explotion") == 0){
                std::cout << "WorkerLoopGame: salgo porque explote "<< std::endl;

                clientDie = true;
                break;
            }

            actionsGame.push_back(a);
        }

        this->actions.clear();

        mutexActions.unlock();

        if(clientDie)
            break;


        //TOMO LAS ACCIONES PARSEADAS DE LOS REQUEST Y LAS
        // APLICO A TODOS LOS ACTORES
        /*for (GameAction* a : actionsGame) {
            for (GameActor* g : gameActors){

            }
        }*/

        //luego de modificar el estado de cada actor hago que vivan
        /*for (GameActor* g : gameActors) {
            g->live();
        }*/
        /* hago que los enemigos caminen */

        for (hordeIt = hordes.begin(); hordeIt != hordes.end(); ++hordeIt){
            std::vector<ActorEnemy*> enemies = hordeIt->second->getEnemies();

            for (auto enemy : enemies) {
                enemy->live();

                /*if(ciclos > 0){
                    gameFinish = true;
                    cause = "enemigos llegaron al final";

                }*/
            }

        }

        /* ahora hago andar las torres */
        //for (auto tower : towers){
            /* verificar colisión con bicho */
            /* si hay colisión */
                /* sumar puntos a torre */
                /* sacar vida a bicho */
        //}

        //GET STATUS GAMES
        statusGame = getGameStatus();

        //NOTIFICO EL ESTADO DEL JUEGO A TODOS LO JUGADORES LUEGO DE LA MODIFICACION DE MODELO
        for (auto it=players.begin(); it!=players.end(); ++it){
            it->second->sendData(statusGame);
        }

        ciclos--;

        actionsGame.clear();//limpio la lista para no ejecutar request viejos
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if(ciclos > 0){
            gameFinish = true;
            cause = "se terminaron los ciclos";
        }


    }

    // ESTA PARTE NO SE SI TIENE SENTIDO,
    // DEBERIA DEE HABER SALIDO TODOS LOS CLIENTES
    std::cout << "WorkerLoopGame: TERMINO PARTIDA CAUSA: "<< cause << std::endl;
    std::cout << "WorkerLoopGame: Hilo donde existe la partida se termino" << std::endl;
    std::cout << "WorkerLoopGame: Aviso que terminó la partida." << std::endl;
    statusGame = MessageFactory::getMatchEndedNotification();
    for (auto it=players.begin(); it!=players.end(); ++it){
        it->second->sendData(statusGame);
    }
}

void WorkerLoopGame::buildGameContext() {
    std::vector<Point> camino1;
    // supongo que recorro una matriz de baldosas de
    // CARTESIAN_TILE_WIDTH x CARTESIAN_TILE_HEIGHT
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

    Horde *horda1 = new Horde();

    for (int x = 0; x < 3; ++x){
        ActorEnemy* enemy = new ActorEnemy();
        enemy->setPath(camino1);
        enemy->setId(x);
        enemy->setCurrentPathPosition(-x * CARTESIAN_TILE_WIDTH / 2);

        horda1->addEnemy(enemy);
    }
    hordes.insert(std::pair<int, Horde*>(0, horda1));

    std::vector<Point> camino2;
    // supongo que recorro una matriz de baldosas de
    // CARTESIAN_TILE_WIDTH x CARTESIAN_TILE_HEIGHT
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


    Horde *horda2 = new Horde();

    for (int x = 0; x < 3; ++x){
        ActorEnemy* enemy = new ActorEnemy();
        enemy->setPath(camino2);
        enemy->setId(x);
        enemy->setCurrentPathPosition(-x * CARTESIAN_TILE_WIDTH / 2);

        horda2->addEnemy(enemy);
    }
    hordes.insert(std::pair<int, Horde*>(1, horda2));


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
    return GameNotification::getStatusMatchNotification(hordes, towers);
}
