#include "GameServer.h"
#include "../common/Socket.h"
#include "../common/TextMessage.h"
#include "../common/Message.h"
#include "../common/MessageFactory.h"
#include "../sdl/Constants.h"
#include "../common/Protocol.h"

GameServer::GameServer(){}

GameServer::~GameServer() {}

void GameServer::addClient(ThreadedQueue<TextMessage> &queue) {
    clients.push_back(queue);
}

void GameServer::notifyAll(std::string message) {
    m.lock();
    TextMessage msg(message);
    for (auto &client : clients) {
        client.get().push(msg);
    }
    m.unlock();
}

void GameServer::processAndNotify(std::string request){
    m.lock();

    /*Message message;
    std::string response;

    message.deserialize(request);
    Json::Value &root = message.getData();

    int op = root[OPERATION_KEY].asInt();

    bool isGlobalNotification = true;

    switch(op){
        *//* non-gaming requests: *//*
        case CLIENT_REQUEST_ACCESS_GAME_MENU:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_NEW_MATCH:{
            response = MessageFactory::getNewMatchNotification(root, matches);
            break;
        }
        *//* this responses are individual *//*
        case CLIENT_REQUEST_ENTER_MATCH:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_GET_MAPS:{
            response = MessageFactory::getExistingMapsNotification();
            break;
        }
        case CLIENT_REQUEST_GET_MATCHES:{
            response = MessageFactory::getExistingMatchesNotification(matches);;
            break;
        }
        *//* gaming requests: *//*
        case CLIENT_REQUEST_PUT_TOWER:{
            response = MessageFactory::getPutTowerNotification(request);
            break;
        }
        case CLIENT_REQUEST_MARK_TILE:{
            response = MessageFactory::getMarkTileNotification(request);
            break;
        }
        default:
            response = request;
    }

    if (isGlobalNotification){
        notifyAllWithoutLock(response);
    } else {
        *//* acá se disparan las notificaciones individuales,
         * por ejemplo, cuando no se puede crear un match o
         * no se puede unir a un match...ver cómo utilizar
         * el mecanismos con socketmanager para obtener el
         * cliente que hace el request en este caso.
         * *//*
    }*/

    m.unlock();
}

void GameServer::notifyAllWithoutLock(std::string message){
    TextMessage msg(message);
    for (auto &client : clients) {
        client.get().push(msg);
    }
}