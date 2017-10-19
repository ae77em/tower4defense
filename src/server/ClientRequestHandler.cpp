#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>

ClientRequestHandler::ClientRequestHandler(Socket &c) : client(c) { }

ClientRequestHandler::~ClientRequestHandler() { }

static uint32_t receiveSize(Socket &sock) {
    uint32_t response;

    int received_bytes = sock.receive(reinterpret_cast<char*>(&response),
            sizeof(uint32_t));

    if (received_bytes == sizeof(uint32_t))
        return response;
    else
        return 0;
}

void ClientRequestHandler::run() {
    char aux_request[2048] = {0};
    uint32_t response_size;
    std::string response;
    std::string request;

    uint32_t request_size = receiveSize(client);
    while (request_size != 0) {
        client.receive(aux_request, request_size);
        request = std::string(aux_request);

        std::cout << "SERVER RECIBIÓ: ";
        std::cout << request << std::endl;

        response.assign("Recibí pedido '");
        response.append(request);
        response.append("' correcamente :).");

        response_size = response.size();
        client.send(reinterpret_cast<char*>(&response_size), sizeof(uint32_t));
        client.send(response.c_str(), response.length());

        request_size = receiveSize(client);
    }
}

