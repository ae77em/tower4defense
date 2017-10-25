#include "Server.h"
#include "../common/Socket.h"
#include "../common/TextMessage.h"

Server::Server(){}

Server::~Server() {}

void Server::addClient(Socket &client){
    clients.push_back(&client);
}

void Server::notifyAll(std::string message) {
    m.lock();
    TextMessage msg(message);
    for (Socket *client : clients){
        msg.sendTo(*client);
    }
    m.unlock();
}
 