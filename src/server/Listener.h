#ifndef LISTENER_H
#define LISTENER_H

#include "../common/Socket.h"
#include "../common/Thread.h"
#include "ClientRequestHandler.h"

#include <vector>
#include <cstdlib>

class Listener : public Thread {
private:
    Socket serverSocket;
    std::vector<ClientRequestHandler*> threads;
    std::vector<Socket*> clients;

    //para proteger la lista de jugadores
     std::mutex& mutexPlayers;

public:
    explicit Listener(uint16_t port, std::mutex& mutexPlayers);
    Listener(const Listener& orig, std::mutex& mutexPlayers);

    void run();

    void shutdown();

private:
    Listener() = delete;
};

#endif /* LISTENER_H */

