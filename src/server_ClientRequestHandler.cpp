#include "server_ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>

ClientRequestHandler::ClientRequestHandler(Socket &c) : client(c) { }

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::run() {
    char aux_request[10] = {0};
    std::string response;
    std::string request;
    bool is_data_received = client.receive(aux_request, 10) == 1;

    request = std::string(aux_request);

    if (is_data_received) {
        try {
            std::cout << "SERVER RECIBIÓ: ";
            std::cout << request << std::endl;

            response.assign("Recibí pedido '");
            response.append(request);
            response.append("' correcamente :).");

        } catch (...) {
            std::cerr
                    << "Error desconocido procesando el request."
                    << std::endl;
        }

        client.send(response.c_str(), response.length());
    }
}

