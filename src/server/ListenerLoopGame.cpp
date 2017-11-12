#include <iostream>
#include "ListenerLoopGame.h"
#include "Request.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"

ListenerLoopGame::ListenerLoopGame(std::list<GameAction *> &a,
                                   std::mutex &m,
                                   ThreadedQueue<Message> &q,
                                    bool& end) :
        actions(a),
        mutexActions(m),
        queueMessagesGame(q),
        endSignal(end){

}

void ListenerLoopGame::run() {
    std::cout << "ListenerLoopGame: "
            "Hilo de sincronizacion de partida arrancando" << std::endl;

    try {
        while (!queueMessagesGame.isAtEnd()) {
            std::cout << "ListenerLoopGame: "
                    "Llego un request de partida" << std::endl;

            Message messageRequest = queueMessagesGame.pop();
            std::string response;

            Request request(messageRequest);
            //hago switch para identificar el tipo de action a aplicar
            // sobre el game loop
            switch (request.getAsInt(OPERATION_KEY)) {
                case GAME_REQUEST_PUT_TOWER: {
                    // poner torre
                    break;
                }
                case GAME_REQUEST_MARK_TILE: {
                    /*int x = request.getAsInt(XCOORD_KEY);
                    int y = request.getAsInt(YCOORD_KEY);
*/
                    break;
                }
                case GAME_REQUEST_CAST_SPELL: {
                    // lanzo hechizo
                    break;
                }
            }

            mutexActions.lock();
            //actions.push_back(new SpecificAction);
            mutexActions.unlock();
        }
    } catch (std::exception) {
        std::cout << "ListenerLoopGame: se rompio cola compartida de acitons"
                  << std::endl;
    }

    //nose si esta parte sirve,
    mutexActions.lock();
    endSignal = true;
    mutexActions.unlock();
    std::cout << "ListenerLoopGame: Se murio" << std::endl;
}