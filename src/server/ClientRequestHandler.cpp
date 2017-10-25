#include "ClientRequestHandler.h"

#include <string>
#include <cstring>
#include <iostream>
#include "Message_Text.h"
#include "ThreadedQueue.h"
#include "SocketManager.h"

#define T ClientRequestHandler
#define M TextMessage
#define Q ThreadedQueue

T::T(Socket &&c) : client(std::move(c)) { }

T::~T() { }

void T::run() {
    SocketManager manager(std::move(client));
    Q<M> &queue = manager.receiveQueue();

    while (! queue.isAtEnd()) {
        M message = queue.pop();

        std::cout << "SERVER RECIBIÓ: ";
        std::cout << message.getMessage() << std::endl;

        std::string response;
        response.assign("Recibí pedido '");
        response.append(message.getMessage());
        response.append("' correctamente");

        M r(response);
        manager.sendQueue().push(response);
    }
}

