#include "Listener.h"
#include "../common/Socket.h"

#include <vector>
#include <cstring>
#include <iostream>

Listener::Listener(uint16_t port) {
    serverSocket.bind(port);
    serverSocket.listen();
}

Listener::Listener(const Listener& orig) { }

Listener::~Listener() { }

void Listener::shutdown(){
    int size = threads.size();
    for (int i = 0; i < size; ++i){
        threads[i]->join();
        delete threads[i];
    }

    serverSocket.shutdown();
}

void Listener::run(){
    try {
        Server server;

        while (true) {
            int fd = serverSocket.accept();
            Socket *client = new Socket(fd);

            std::cout   << "nueva conexion entrante. FD: "
                        << std::to_string(fd)
                        << std::endl;

            // ClientRequestHandler se encarga de agregar el cliente al server
            ClientRequestHandler *rp = new ClientRequestHandler(*client, server);

            threads.push_back(rp);

            rp->start();
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}
