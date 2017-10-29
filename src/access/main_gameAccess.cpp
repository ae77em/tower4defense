#include <gtkmm.h>
#include <iostream>
#include "AccessGameHandler.h"
#include "../common/SharedBuffer.h"
#include "../common/Socket.h"
#include "Listener.h"
#include "Sender.h"
#include "../common/MessageFactory.h"

int main(int argc, char **argv) {
    GameAccess gameAccess;
    SharedBuffer toReceive;
    SharedBuffer toSend;
    std::string host;
    uint16_t port;

    host = std::string(argv[1]);
    port = atoi(argv[2]);

    Socket client;

    client.connect(host.c_str(), port);

    std::string mapsRequest = MessageFactory::getMapsRequest(client.getSocket());
    //std::string matchesRequest = MessageFactory::getMatchesRequest(client.getSocket());

    toSend.addData(mapsRequest);
    //toSend.addData(matchesRequest);

    Listener listener(client, gameAccess);
    Sender sender(client, toSend);

    gameAccess.start();

    time_t start = time(0);
    // wait for  2 seconds
    while (difftime( time(0), start) < 2 ){}

    listener.start();
    sender.start();

    listener.join();
    sender.join();
    gameAccess.join();

    client.shutdown();
    client.close();

    return 0;
}