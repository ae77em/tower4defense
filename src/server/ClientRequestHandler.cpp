#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>

ClientRequestHandler::ClientRequestHandler(Socket &c) : client(c) { }

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::run() {
    char aux_request[2048] = {0};
    uint32_t request_size;
    uint32_t response_size;
    std::string response;
    std::string request;
    bool is_data_received =
            client.receive(
                        reinterpret_cast<char*>(&request_size),
                        sizeof(uint32_t)
                    ) == sizeof(uint32_t);

    while (is_data_received) {
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

        is_data_received =
            client.receive(
                        reinterpret_cast<char*>(&request_size),
                        sizeof(uint32_t)
                    ) == sizeof(uint32_t);
    }
}

