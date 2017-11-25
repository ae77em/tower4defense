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
        Message message;
        try {
            std::cout
                    << "CRH: cliente: "
                    << client->getId()
                    << " Esperando Mensaje" << std::endl;

            TextMessage textMessage("");
            std::string requestSerialized
                    = textMessage.receiveFrom(*client).getMessage();

            message.deserialize(requestSerialized);

            int operationKey = MessageFactory::getOperation(message);

            std::cout << "CRH: cliente: "
                      << client->getId()
                      << " Recibio y despacho Mensaje: "
                      << message.toString()
                      << std::endl;

            if (operationKey == CLIENT_REQUEST_END_GAME) {
                std::cout
                        << "CRH: cliente: "
                        << client->getId()
                        << " requirio finalizar su comuc. "
                        << std::endl;
                break;
            }
        }catch (std::runtime_error){
            Message message;
            int clientId = client->getId();
            std::string notif
                = MessageFactory::getClientEndConectionNotification(clientId);
            message.deserialize(notif);
            queueSharedMessage.push(message);
            std::cout
                    << "CRH: cliente: "
                    << client->getId()
                    <<" TERMINO CONEXION"
                    <<std::endl;
            break;
        }
        queueSharedMessage.push(message);
    }

    std::cout << "CRH: cliente: "
              << client->getId() <<" termino su CTH thread"<<std::endl;
}

