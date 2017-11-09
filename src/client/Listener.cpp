#include <iostream>
#include "Listener.h"
#include "../common/TextMessage.h"
#include "../common/SharedBuffer.h"
#include "../common/MessageFactory.h"
#include "../common/Message.h"
#include "../common/Protocol.h"

Listener::Listener(Socket *s, GameAccessWindow &ga, SharedBuffer *bfr)
        : server(s), gameAccess(ga), buffer(bfr) { }

Listener::~Listener() {}

void Listener::run(){
    try {
        std::string dataFromServer;
        TextMessage textMessage("");

        std::cout << "llegue al listener---" << std::endl;

        while (true) {
            dataFromServer = textMessage.receiveFrom(*server).getMessage();

            Message message;
            std::string response = "";

            message.deserialize(dataFromServer);

            int op = MessageFactory::getOperation(message);

            std::cout << "llego operación: " << std::to_string(op) << std::endl;

            switch(op){
                /*
                 * ACCESS OPERATIONS
                 */
                case SERVER_NOTIFICATION_CLIENT_ID:{
                    int clientId = MessageFactory::getClientId(message);
                    gameAccess.setClientId(clientId);
                    break;
                }
                case SERVER_NOTIFICATION_GET_ALL_MAPS:{
                    std::vector<std::string> maps;
                    maps = MessageFactory::getMaps(message);
                    gameAccess.addMapsToCombo(maps);
                    break;
                }
                case SERVER_NOTIFICATION_GET_ALL_MATCHES:{
                    std::vector<std::string> matches;
                    matches = MessageFactory::getMatches(message);
                    gameAccess.addMatchesToCombo(matches);
                    break;
                }
                case SERVER_NOTIFICATION_NEW_MATCH:{
                    std::string mapName = MessageFactory::getMapName(message);
                    std::string matchName = MessageFactory::getMatchName(message);

                    gameAccess.addMatchToCombo(mapName, matchName);
                    break;
                }
                case SERVER_NOTIFICATION_GET_UNAVAILABLE_ELEMENTS:{
                    std::list<std::string> elements = MessageFactory::getElements(message);
                    gameAccess.setAvailableElements(elements);
                    break;
                }
                case SERVER_NOTIFICATION_START_MATCH:{
                    std::list<std::string> elements = MessageFactory::getElements(message);

                    gameAccess.startMatch();
                    break;
                }
                case SERVER_NOTIFICATION_ENTER_EXISTING_MATCH:{
                    int clientId = MessageFactory::getClientId(message);
                    std::list<std::string> elements = MessageFactory::getElements(message);

                    if (gameAccess.getClientId() != clientId){
                        gameAccess.setAvailableElementsForJoin(elements);
                    }
                    break;
                }
                case SERVER_NOTIFICATION_ENTERED_MATCH:{
                    int clientId = MessageFactory::getClientId(message);
                    std::string matchName = MessageFactory::getMatchName(message);
                    gameAccess.setJoinedToMatch(clientId, matchName);
                    break;
                }
                /*
                 * GAME OPERATIONS
                 * Todas aquellas operaciones que no corresponden con algo que se hace en
                 * el login, lo consideramos acción de juego, por ende agregamos la acción al
                 * buffer compartido con el thread donde se desarrolla el mismo. No se si es
                 * la mejor solución, pero es la que me surgió primero, teniendo en cuenta que
                 * las cosas las tenía resueltas por separado...
                 */
                default:
                    std::cout << "Llegó notificación no de acceso...la mando la juego..." << std::endl;
                    std::cout << dataFromServer << std::endl;
                    buffer->addData(dataFromServer);
            }

            std::cout << response << "dataFromServer: " << dataFromServer << std::endl;
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}