#include <iostream>
#include "Listener.h"
#include "../common/TextMessage.h"
#include "../common/SharedBuffer.h"
#include "../common/MessageFactory.h"
#include "../common/Message.h"
#include "../common/Protocol.h"

Listener::Listener(Socket &s, GameAccess &ga) : server(s), gameAccess(ga) { }

Listener::~Listener() {}

void Listener::run(){
    try {
        std::string dataFromServer;
        TextMessage textMessage("");

        std::cout << "llegue al listener---" << std::endl;

        while (true) {
            dataFromServer = textMessage.receiveFrom(server).getMessage();

            Message message;
            std::string response = "";

            message.deserialize(dataFromServer);

            int op = MessageFactory::getOperation(message);

            std::cout << "llego operación: " << std::to_string(op) << std::endl;

            switch(op){
                case SERVER_NOTIFICATION_CLIENT_ID:{
                    int clientId = MessageFactory::getClientId(message);
                    gameAccess.setClientId(clientId);
                    break;
                }
                case SERVER_NOTIFICATION_GET_MAPS:{
                    std::vector<std::string> maps;
                    maps = MessageFactory::getMaps(message);
                    gameAccess.addMapsToCombo(maps);
                    break;
                }
                case SERVER_NOTIFICATION_NEW_MATCH:{
                    std::string mapName = MessageFactory::getMapName(message);
                    std::string matchName = MessageFactory::getMatchName(message);

                    gameAccess.addMatchToCombo(mapName, matchName);
                    break;
                }
                case SERVER_NOTIFICATION_GET_ELEMENTS:{
                    std::list<std::string> elements = MessageFactory::getElements(message);

                    gameAccess.addElementsToCombo(elements);
                    break;
                }
                default:
                    response = "notificación del server no reconocida";
            }

            std::cout << response << "dataFromServer: " << dataFromServer << std::endl;
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}