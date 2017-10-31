#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>
#include "../common/TextMessage.h"
#include "../common/ThreadedQueue.h"
#include "../common/SocketManager.h"
#include "../common/Protocol.h"
#include "../common/MessageFactory.h"
#include "../common/Message.h"
#include <sstream>

ClientRequestHandler::ClientRequestHandler(Socket &c, ThreadedQueue<Message> &th)
                                                                :client(c)
                                                                , queueSharedMessage(th) {}

ClientRequestHandler::~ClientRequestHandler() { }

void ClientRequestHandler::sendData(std::string data){
    TextMessage d( data );
    d.sendTo(client);
}

void ClientRequestHandler::sendClientId() {
    string message = MessageFactory::getClientIdNotification(client.getSocket() );
    sendData(message);
}

void ClientRequestHandler::run() {
    sendClientId();
    std::string data;

    while(true){
        std::cout << "CRH: cliente: "<< client.getSocket() <<" Esperando Mensaje"<<std::endl;

        TextMessage textMessage("");
        std::string requestSerialized = textMessage.receiveFrom(client).getMessage();

        Message message;
        message.deserialize(requestSerialized);

        int operationKey = MessageFactory::getOperation(message);

        std::cout << "CRH: cliente: "<< client.getSocket() <<" Recibio y despacho Mensaje: "<< message.toString() <<std::endl;

        if(operationKey == CLIENT_REQUEST_END_GAME){
            std::cout << "CRH: cliente: "<< client.getSocket() <<" requirio finalizar su comuc. "<<std::endl;
            break;
        }
        queueSharedMessage.push(message);
    }
    std::cout << "CRH: cliente: "<< client.getSocket() <<" termino su CTH thread"<<std::endl;
}

