#ifndef SERVER_CLIENT_REQUEST_HANDLER_H
#define SERVER_CLIENT_REQUEST_HANDLER_H

#include "../common/Thread.h"
#include "../common/Socket.h"
#include "GameServer.h"

class ClientRequestHandler : public Thread {
private:
    char op;
    SocketManager client;
    GameServer &server;

public:
    explicit ClientRequestHandler(Socket &client, GameServer &s);
    virtual ~ClientRequestHandler();

    void run();

private:
    ClientRequestHandler() = delete;
    ClientRequestHandler(const ClientRequestHandler& orig) = delete;
};

#endif /* SERVER_CLIENT_REQUEST_HANDLER_H */

