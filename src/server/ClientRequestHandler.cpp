#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>
#include "Message_Text.h"

ClientRequestHandler::ClientRequestHandler(Socket &c) : client(c) { }

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::run() {
    TextMessage msg("");
    try { msg = receiveFrom(client); } catch (std::exception) { return; }
    while (true) {
        std::cout << "SERVER RECIBIÓ: ";
        std::cout << msg.getMessage() << std::endl;

        std::string response;
        response.assign("Recibí pedido '");
        response.append(msg.getMessage());
        response.append("' correcamente :).");

        TextMessage r(response);
        r.sendThrough(client);

        try { msg = receiveFrom(client); } catch (std::exception) { return; }
    }
}

