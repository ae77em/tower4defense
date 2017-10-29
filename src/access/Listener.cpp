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
        TextMessage message("");

        while (true) {
            dataFromServer = message.receiveFrom(server).getMessage();

            Message message;
            std::string response;

            message.deserialize(dataFromServer);
            Json::Value &root = message.getData();

            int op = root[OPERATION_KEY].asInt();

            switch(op){
                case SERVER_NOTIFICATION_GET_MAPS:{
                    std::vector<std::string> maps;
                    for (Json::Value &map : root["maps"]){
                        maps.push_back(map.asString());
                    }

                    gameAccess.addMapsToCombo(maps);
                    break;
                }
                case SERVER_NOTIFICATION_NEW_MATCH:{
                    std::string mapName = root["mapName"].asString();
                    std::string matchName = root["matchName"].asString();

                    gameAccess.addMatchToCombo(mapName, matchName);
                    break;
                }
                default:
                    response = "no reconocida";
            }

            std::cout << "dataFromServer: " << dataFromServer << std::endl;
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}