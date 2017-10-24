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
        std::cout << msg.getMessage();
        std::cout << "' desde el cliente ";
        std::cout << std::to_string(client.get_socket());
        std::cout << std::endl;

        std::string response;
        response.assign("Recibí pedido '");
        response.append(msg.getMessage());
        response.append("' desde el cliente ");
        response.append(std::to_string(client.get_socket()));
        response.append(" correctamente :).");

        TextMessage r(response);
        r.sendThrough(client);

        try { msg = receiveFrom(client); } catch (std::exception) { return; }
    }
}

