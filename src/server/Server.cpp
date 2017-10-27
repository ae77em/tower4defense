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

void Server::processAndNotifyAll(std::string request){
    m.lock();

    Message message;
    std::string response;

    message.deserialize(request);
    Json::Value &root = message.getData();

    int op = root[OPERATION_KEY].asInt();

    switch(op){
        case CLIENT_REQUEST_PUT_TOWER:{
            response = MessageFactory::getPutTowerNotification(root);
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