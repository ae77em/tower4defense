#include <gtkmm.h>
#include <iostream>
#include "AccessGameHandler.h"
#include "../common/SharedBuffer.h"
#include "../common/Socket.h"
#include "Listener.h"
#include "Sender.h"
#include "../common/MessageFactory.h"

int main(int argc, char **argv) {
    SharedBuffer toReceive;
    SharedBuffer toSend;
    std::string host;
    uint16_t port;

    host = std::string(argv[1]);
    port = atoi(argv[2]);

    Socket client;

    client.connect(host.c_str(), port);

    GameAccess gameAccess(client, host, port);
    Listener listener(client, gameAccess);
    Sender sender(client, toSend);

    gameAccess.start();

    time_t start = time(0);
    // wait for 1 second, while the window is setted
    while (difftime(time(0), start) < 1 ){}

    listener.start();
    // sends the initial requests, and inmediatly finishes him
    sender.start();

    std::string mapsRequest = MessageFactory::getExistingMapsRequest(client.getSocket());
    toSend.addData(mapsRequest);

    std::string matchesRequest = MessageFactory::getExistingMatchesRequest(client.getSocket());
    toSend.addData(matchesRequest);

    toSend.setClientProcessEnded(true);
    sender.join();

    listener.join();
    gameAccess.join();

    client.shutdown();
    client.close();

    return 0;
}