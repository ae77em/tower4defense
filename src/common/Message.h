#include "../common/Socket.h"

#ifndef GAMEMESSAGE_H
#define GAMEMESSAGE_H

class Message {
public:
    virtual void sendThrough(Socket &sock) = 0;
    virtual void receiveFrom(Socket &sock) = 0;
};

#endif
