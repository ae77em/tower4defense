#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>
#include "../common/TextMessage.h"
#include "../common/ThreadedQueue.h"
#include "../common/SocketManager.h"

ClientRequestHandler::ClientRequestHandler(Socket &c, Server &s) : client(std::move(c)), server(s) {}

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::run() {
    server.addClient(client.sendQueue());

    ThreadedQueue<TextMessage> &queue = client.receiveQueue();
    while (! queue.isAtEnd()) {
        TextMessage msg = queue.pop();

        std::cout << "SERVER RECIBIÓ: ";
        std::cout << msg.getMessage();
        std::cout << std::endl;

        std::string response = msg.getMessage();
        response.assign("Recibí pedido '");
        response.append(msg.getMessage());
        response.append(" correctamente :).");

        server.notifyAll(response);
    }
}

