#include "Listener.h"
#include "Server.h"

#include <vector>
#include <cstring>
#include <iostream>

Listener::Listener(uint16_t port, std::mutex& aMutexPlayers):mutexPlayers(aMutexPlayers) {
    serverSocket.bind(port);
    serverSocket.listen();
}

Listener::Listener(const Listener& orig, std::mutex& aMutexPlayers):mutexPlayers(aMutexPlayers) { }

void Listener::shutdown(){
    int size = threads.size();
    for (int i = 0; i < size; ++i){
        threads[i]->join();
        delete threads[i];
    }

    serverSocket.shutdown();
}

void Listener::run(){
    //todos los ClientRequestHandler cargaran sus mensajes en esta cola y el servidor estara
    //esperando a tomarlos
    ThreadedQueue<Message> threadedQueue;
    Server server(mutexPlayers,threadedQueue);

    try {
        server.start();

       while (true) {
            std::cout   << "Listener escuchando esperando conexion: "<< std::endl;

            int fd = serverSocket.accept();
            Socket *client = new Socket(fd);

            std::cout   << "nueva conexion entrante. FD: "
                        << std::to_string(fd)
                        << std::endl;

           //ClientRequestHandler se encarga solamente de tomar los request del client
           server.createAndRunPlayer(client);
        }
    } catch (std::exception) {
        std::cout<< "Listener: Cachenado excepcion, se corto accept"<< std::endl;
    }
    std::cout   << "Listener: Esperando finalizacion de todas las comunicaciones"<< std::endl;

    threadedQueue.close();
    server.join();

    std::cout   << "Listener: Comunicaciones finalizadas, listener terminando su hilo"<< std::endl;
}
