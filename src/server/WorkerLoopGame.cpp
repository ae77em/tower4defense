#include "WorkerLoopGame.h"

WorkerLoopGame::WorkerLoopGame(std::vector<ServerPlayer*>& p,
                               std::list<ActionGame*>& a,
                               std::mutex& m):
                                                players(p),
                                                actions(a),
                                                mutexActions(m){
}

void WorkerLoopGame::run(){
    std::cout << "WorkerLoopGame: Hilo donde existe la partida arrancando" << std::endl;
    unsigned int ciclos = 300;

    //while(isRunning()){
    while (ciclos > 0){
        //std::list<ActionGame*> obtainedActions = getActions();
        std::list<ActionGame *> actionsGame;

        mutexActions.lock();
        for (ActionGame* a : this->actions) {
            actionsGame.push_back(a);
        }
        mutexActions.unlock();

        //TOMO LAS ACCIONES PARSEADAS DE LOS REQUEST Y LAS APLICO A TODOS LOS ACTORES
        /*for (ActionGame* a : actionsGame) {
            for (GameActor* g : gameActors){

            }
        }*/

        //luego de modificar el estado de cada actor hago que vivan
        for (GameActor* g : gameActors) {
            g->live();
        }

        //GET STATUS GAMES
        std::string statusGame = "statusGame";

        //NOTIFICO EL ESTADO DEL JUEGO A TODOS LO JUGADORES LUEGO DE LA MODIFICACION DE MODELO
        for(ServerPlayer* s : players){
            s->sendData(statusGame);
        }

        ciclos--;
    }

    std::cout << "WorkerLoopGame: Hilo donde existe la partida se termino" << std::endl;
}

void WorkerLoopGame::buildContextGame() {
    //levanto el escenario y la data de todos los mounstruos
}
