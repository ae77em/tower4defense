#ifndef SERVER_CLIENT_REQUEST_HANDLER_H
#define SERVER_CLIENT_REQUEST_HANDLER_H

#include "../common/Thread.h"
#include "../common/Socket.h"
#include "Server.h"

class ClientRequestHandler : public Thread {
private:
    char op;
    Socket &clientSocket;
    Server &server;

public:
    explicit ClientRequestHandler(Socket &client, Server &s);
    virtual ~ClientRequestHandler();

    void run();

private:
    ClientRequestHandler() = delete;
    ClientRequestHandler(const ClientRequestHandler& orig) = delete;
};

#endif /* SERVER_CLIENT_REQUEST_HANDLER_H */

