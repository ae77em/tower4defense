#include "WorkerLoopGame.h"
#include "../common/Point.h"
#include "../sdl/Constants.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "game-actors/enemies/Horde.h"
#include "../common/MessageFactory.h"
#include "GameNotification.h"
#include "../common/modelo/Mapa.h"
#include "../common/Protocol.h"

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
                               std::mutex& m,
                                model::Mapa& mp):
                                                players(p),
                                                actions(a),
                                                mutexActions(m),
                                                map(mp) { }

void WorkerLoopGame::run(){
    std::cout << "WorkerLoopGame: Hilo donde "
            "existe la partida arrancando" << std::endl;
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

        if( isTimeToCreateHorde() ){
            createHordeAndNotify();
        }

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
                std::cout << "WorkerLoopGame: "
                        "salgo porque explote "<< std::endl;

                clientDie = true;
                break;
            }  else if (a->action.compare("put-tower")){
                ActorTower *tower = new ActorTower();
                tower->setPosition(a->x, a->y);
                towers.push_back( tower );
            }
            actionsGame.push_back(a);
        }

        this->actions.clear();

        mutexActions.unlock();

        if(clientDie)
            break;

        for (hordeIt = hordes.begin(); hordeIt != hordes.end(); ++hordeIt){
            std::vector<ActorEnemy*> enemies = hordeIt->second->getEnemies();

            for (auto enemy : enemies) {
                enemy->live();
            }
        }

        /* ahora hago andar las torres */
        for (auto tower : towers){
            for (hordeIt = hordes.begin(); hordeIt != hordes.end(); ++hordeIt) {
                tower->live(hordeIt->second);
            }
        }

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
    std::cout << "WorkerLoopGame: Hilo donde "
            "existe la partida se termino" << std::endl;
    std::cout << "WorkerLoopGame: Aviso que terminó la partida." << std::endl;
    statusGame = MessageFactory::getMatchEndedNotification();
    for (auto it=players.begin(); it!=players.end(); ++it){
        it->second->sendData(statusGame);
    }
}

void WorkerLoopGame::buildGameContext() {
    timeBetweenHordeCreation = 5000; //milisegundos
    timeLastHordeCreation = 0;

    hordeType.push_back((int)ENEMY_ABMONIBLE);
    hordeType.push_back((int)ENEMY_BLOOD_HAWK);
    hordeType.push_back((int)ENEMY_SPECTRE);

    paths = map.getCaminos();

    hordeId = 0;

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

bool WorkerLoopGame::isTimeToCreateHorde() {
    time_t now;
    time(&now);

    return (now - timeLastHordeCreation) > timeBetweenHordeCreation;
}

void WorkerLoopGame::setTimeCreationHorde(){
    time_t now;
    time(&now);

    timeLastHordeCreation = now;
}

void WorkerLoopGame::createHordeAndNotify() {
    setTimeCreationHorde();

    time_t now;
    time(&now);

    unsigned hordeIndex = now % hordeType.size();
    int nextHordeType = hordeType.at(hordeIndex);

    unsigned pathIndex = now % paths.size();
    std::vector<Point> path = static_cast<std::vector<Point> &&>(paths.at(pathIndex));

    Horde *h = Horde::createHorde(nextHordeType, 3, path);

    hordes.insert(std::make_pair(hordeId, h));

    std::string statusGame =
        GameNotification::getNewHordeNotification(hordeId, nextHordeType, 3);

    for (auto it=players.begin(); it!=players.end(); ++it){
        it->second->sendData(statusGame);
    }

    ++hordeId;
}
