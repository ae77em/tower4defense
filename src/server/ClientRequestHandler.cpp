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

T::T(Socket &&c, Juego &juego) : client(std::move(c)), juego(juego) { }

T::~T() { }

void T::run() {
    SocketManager manager(std::move(client));
    juego.subscribirCliente(manager.sendQueue());

    Q<M> &queue = manager.receiveQueue();
    while (! queue.isAtEnd()) {
        M message = queue.pop();

        std::cout << "SERVER RECIBIÓ: ";
        std::cout << message.getMessage() << std::endl;

        if (message.getMessage() == "cambio tile...")
            juego.agregarTorre(1, 2);
    }
}

