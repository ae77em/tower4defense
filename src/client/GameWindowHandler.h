#ifndef TP4_TOWERDEFENSE_GAMEWINDOWHANDLER_H
#define TP4_TOWERDEFENSE_GAMEWINDOWHANDLER_H

#include "../common/Thread.h"
#include "../common/Socket.h"

class GameWindowHandler {
private:
    const std::string &host;
    const uint16_t &port;

public:
    GameWindowHandler(const std::string &host, const uint16_t &port);
    virtual ~GameWindowHandler();

    void run(Socket &socket, int clientId);
};


#endif //TP4_TOWERDEFENSE_GAMEWINDOWHANDLER_H
