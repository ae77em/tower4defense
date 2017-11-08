#include <iostream>
#include "ListenerLoopGame.h"

ListenerLoopGame::ListenerLoopGame(std::list<GameAction*>& a,
                                   std::mutex& m,
                                   ThreadedQueue<Message>& q):
                                        actions(a),
                                        mutexActions(m),
                                        queueMessagesGame(q){

}

void ListenerLoopGame::run(){
    std::cout << "ListenerLoopGame: Hilo de sincronizacion de partida arrancando" << std::endl;

    try {
        while (!queueMessagesGame.isAtEnd()) {
            std::cout << "ListenerLoopGame: Llego un request de partida" << std::endl;

            Message messageRequest = queueMessagesGame.pop();
            std::string response;

            //hago swirh para identificar el tipo de action a aplicar sobre el game loop

            mutexActions.lock();
            //actions.push_back(new SpecificAction);
            mutexActions.unlock();
        }
    } catch (std::exception) {
        std::cout << "ListenerLoopGame: se rompio cola compartida de acitons" << std::endl;
    }
    std::cout << "ListenerLoopGame: Se murio" << std::endl;
}