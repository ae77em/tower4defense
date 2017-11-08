#include "WorkerLoopGame.h"
#include "../common/Point.h"
#include "../sdl/Constants.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "../common/MessageFactory.h"

#include <iostream>
#include <chrono>
#include <thread>

WorkerLoopGame::WorkerLoopGame(std::vector<ServerPlayer*>& p,
                               std::list<GameAction*>& a,
                               std::mutex& m):
                                                players(p),
                                                actions(a),
                                                mutexActions(m){
}

void WorkerLoopGame::run(){
    std::cout << "WorkerLoopGame: Hilo donde existe la partida arrancando" << std::endl;
    unsigned int ciclos = 30;

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
        for (GameActor* g : gameActors) {
            g->live();
        }
        //GET STATUS GAMES
        std::string statusGame = getGameStatus();

        //NOTIFICO EL ESTADO DEL JUEGO A TODOS LO JUGADORES LUEGO DE LA MODIFICACION DE MODELO
        for(ServerPlayer* s : players){
            s->sendData(statusGame);
        }

        ciclos--;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "WorkerLoopGame: Hilo donde existe la partida se termino" << std::endl;
}

void WorkerLoopGame::buildGameContext() {
    std::vector<Point> camino;
    // supongo que recorro una matriz de baldosas de CARTESIAN_TILE_WIDTH x CARTESIAN_TILE_HEIGHT
    camino.push_back(Point(0 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(1 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(2 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(3 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(4 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(4 * CARTESIAN_TILE_WIDTH,6 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(4 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(5 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(6 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(7 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(8 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(9 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(10 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(11 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(12 * CARTESIAN_TILE_WIDTH,7 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(12 * CARTESIAN_TILE_WIDTH,6 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(12 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(13 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(14 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(15 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));
    camino.push_back(Point(16 * CARTESIAN_TILE_WIDTH,5 * CARTESIAN_TILE_HEIGHT));

    for (int x = 0; x < 5; ++x){
        ActorEnemy* enemy = new ActorEnemy();
        enemy->setPath(camino);
        enemy->setCurrentPathPosition(-x * CARTESIAN_TILE_WIDTH / 2);

        gameActors.push_back((GameActor*)enemy);
    }
}

std::string WorkerLoopGame::getGameStatus() {
    //despues ver como parsear todos los actores
    return MessageFactory::getStatusMatchNotification(gameActors);
}
