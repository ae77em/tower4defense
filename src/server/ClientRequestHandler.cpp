#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>

#include "../common/TextMessage.h"
#include "../common/MessageFactory.h"
#include "../common/Message.h"
#include "../common/Protocol.h"

#include "ClientRequestHandler.h"

ClientRequestHandler::ClientRequestHandler(Socket* c,
                                           ThreadedQueue<Message> &th)
                        :client(c), queueSharedMessage(th) {}

void ClientRequestHandler::sendData(std::string data){
    try {
        TextMessage d(data);
        d.sendTo(*client);
    } catch (...){
        std::cerr
                << "Ocurrió una excepción al enviar información al cliente "
                << client->getId()
                << ". Es posible que el cliente se haya desconectado."
                << std::endl;

    }
}

void ClientRequestHandler::sendClientId() {
    std::string message =
            MessageFactory::getClientIdNotification(client->getId() );
    sendData(message);
}

void ClientRequestHandler::run() {
    sendClientId();
    std::string data;

    while (true) {
        std::string incoming_string;
        try {
            TextMessage incoming = TextMessage::receiveFrom(*client);
            incoming_string = incoming.getMessage();

        // Si el cliente cierra el socket, receiveFrom lanza una excepcion
        } catch (std::runtime_error) { break; }

        Message request;
        request.deserialize(incoming_string);
        queueSharedMessage.push(request);
    }

    // Notificar al servidor que el cliente se desconecto
    std::string notif =
        MessageFactory::getClientEndConectionNotification(
                client->getId());
    Message desconectado;
    desconectado.deserialize(notif);
    queueSharedMessage.push(desconectado);
}

