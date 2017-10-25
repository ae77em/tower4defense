#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>
#include "../common/TextMessage.h"

ClientRequestHandler::ClientRequestHandler(Socket &c, Server &s) : clientSocket(c), server(s) { }

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::run() {
    TextMessage msg("");
    try { msg = receiveFrom(clientSocket); } catch (std::exception) { return; }
    while (true) {
        std::cout << "SERVER RECIBIÓ: ";
        std::cout << msg.getMessage() << std::endl;

        std::string response = msg.getMessage();
        response.assign("Recibí pedido '");
        response.append(response);
        response.append("' correcamente :).");

        /*TextMessage r(response);
        r.sendThrough(client);*/

        server.notifyAll(response);

        try { msg = receiveFrom(clientSocket); } catch (std::exception) { return; }
    }
}

