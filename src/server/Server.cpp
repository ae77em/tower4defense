#include "Server.h"
#include "../common/Socket.h"
#include "../common/TextMessage.h"
#include "../common/Message.h"
#include "../common/MessageFactory.h"
#include "../sdl/Constants.h"
#include "../common/Protocol.h"

Server::Server(){}

Server::~Server() {}

void Server::addClient(ThreadedQueue<TextMessage> &queue) {
    clients.push_back(queue);
}

void Server::notifyAll(std::string message) {
    m.lock();
    TextMessage msg(message);
    for (auto &client : clients) {
        client.get().push(msg);
    }
    m.unlock();
}

void Server::processAndNotify(std::string request){
    m.lock();

    Message message;
    std::string response;

    message.deserialize(request);
    Json::Value &root = message.getData();

    int op = root[OPERATION_KEY].asInt();

    switch(op){
        /* non-gaming requests: */
        case CLIENT_REQUEST_ACCESS_GAME_MENU:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_NEW_MATCH:{
            response = request;
            break;
        }
        /* this responses are individual */
        case CLIENT_REQUEST_ENTER_EXISTING_MATCH:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_GET_MAPS:{
            response = request;
            break;
        }

        /* gaming requests: */
        case CLIENT_REQUEST_PUT_TOWER:{
            response = MessageFactory::getPutTowerNotification(root);
            break;
        }
        case CLIENT_REQUEST_MARK_TILE:{
            response = MessageFactory::getMarkTileNotification(root);
            break;
        }
        default:
            response = request;
    }

    notifyAllWithoutLock(response);

    m.unlock();
}

void Server::notifyAllWithoutLock(std::string message){
    TextMessage msg(message);
    for (auto &client : clients) {
        client.get().push(msg);
    }
}