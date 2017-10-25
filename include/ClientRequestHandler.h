#ifndef SERVER_CLIENT_REQUEST_HANDLER_H
#define SERVER_CLIENT_REQUEST_HANDLER_H

#include "Thread.h"
#include "Socket.h"
#include "Juego.h"

class ClientRequestHandler : public Thread {
private:
    Socket client;
    Juego &juego;

public:
    explicit ClientRequestHandler(Socket &&client, Juego &juego);
    virtual ~ClientRequestHandler();

    void run();

private:
    ClientRequestHandler() = delete;
    ClientRequestHandler(const ClientRequestHandler& orig) = delete;
};

#endif /* SERVER_CLIENT_REQUEST_HANDLER_H */

