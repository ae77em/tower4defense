#include "Listener.h"
#include "Socket.h"
#include "Exceptions.h"

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
    int size = clients.size();
    for (int i = 0; i < size; ++i){
        clients[i]->shutdown();
        delete clients[i];
    }

    size = threads.size();
    for (int i = 0; i < size; ++i){
        threads[i]->join();
        delete threads[i];
    }

    server.shutdown();
}

void Listener::run(){
    try {
        while (true) {
            int fd = server.accept();
            Socket *client = new Socket(fd);
            std::cout << "nueva conexion entrante. FD: " << std::to_string(fd) << std::endl;
            ClientRequestHandler *rp = new ClientRequestHandler(*client);

            clients.push_back(client);
            threads.push_back(rp);

            rp->start();
        }
    } catch (accept_exception ex) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}
