#include "Listener.h"
#include "Socket.h"

#include <vector>
#include <cstring>
#include <iostream>

Listener::Listener(uint16_t port) {
    server.bind(port);
    server.listen();
}

Listener::Listener(const Listener& orig) { }

Listener::~Listener() { }

void Listener::shutdown(){
    int size = threads.size();
    for (int i = 0; i < size; ++i){
        threads[i]->join();
        delete threads[i];
    }

    server.shutdown();
}

void Listener::run(){
    try {
        while (true) {
            ClientRequestHandler *rp =
                    new ClientRequestHandler(Socket(server.accept()));

            threads.push_back(rp);

            rp->start();
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}
