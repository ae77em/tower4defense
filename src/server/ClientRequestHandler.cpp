#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>
#include "../common/TextMessage.h"

ClientRequestHandler::ClientRequestHandler(Socket &c, Server &s) : clientSocket(c), server(s) { }

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::run() {
    TextMessage msg("");
    try { msg = msg.receiveFrom(clientSocket); } catch (std::exception) { return; }
    while (true) {
        std::cout << "SERVER RECIBIÓ: ";
        std::cout << msg.getMessage();
        std::cout << "' desde el cliente ";
        std::cout << std::to_string(clientSocket.get_socket());
        std::cout << std::endl;

        std::string response = msg.getMessage();
        response.assign("Recibí pedido '");
        response.append(msg.getMessage());
        response.append("' desde el cliente ");
        response.append(std::to_string(clientSocket.get_socket()));
        response.append(" correctamente :).");

        /*TextMessage r(response);
        r.sendThrough(client);*/

        server.notifyAll(response);

        try { msg = msg.receiveFrom(clientSocket); } catch (std::exception) { return; }
    }
}

