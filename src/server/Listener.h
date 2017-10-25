#ifndef LISTENER_H
#define LISTENER_H

#include "../common/Socket.h"
#include "ClientRequestHandler.h"

#include <vector>
#include <cstdlib>

class Listener : public Thread {
private:
    Socket serverSocket;
    std::vector<ClientRequestHandler*> threads;
    std::vector<Socket*> clients;

public:
    explicit Listener(uint16_t port);
    Listener(const Listener& orig);
    virtual ~Listener();

    void run();

    void shutdown();

private:
    Listener() = delete;
};

#endif /* LISTENER_H */

