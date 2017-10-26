#include "Server.h"
#include "../common/Socket.h"
#include "../common/TextMessage.h"

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
 
