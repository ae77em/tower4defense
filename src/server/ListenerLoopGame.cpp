#include <iostream>
#include "ListenerLoopGame.h"
#include "Request.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"

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

            Request request(messageRequest);
            //hago switch para identificar el tipo de action a aplicar sobre el game loop
            switch (request.getAsInt(OPERATION_KEY)){
                case CLIENT_REQUEST_PUT_TOWER:{
                    // poner torre
                    break;
                }
                case CLIENT_REQUEST_MARK_TILE:{
                    // marcar zona de construcción
                    break;
                }
                case CLIENT_REQUEST_CAST_SPELL:{
                    // lanzo hechizo
                    break;
                }
            }

            mutexActions.lock();
            //actions.push_back(new SpecificAction);
            mutexActions.unlock();
        }
    } catch (std::exception) {
        std::cout << "ListenerLoopGame: se rompio cola compartida de acitons" << std::endl;
    }
    std::cout << "ListenerLoopGame: Se murio" << std::endl;
}