#include <gtkmm.h>
#include <iostream>
#include "../common/SharedBuffer.h"
#include "../common/Socket.h"
#include "ListenerAccess.h"
#include "SenderAccess.h"
#include "../common/MessageFactory.h"
#include "../common/Utils.h"
#include "../common/TextMessage.h"

int main(int argc, char **argv) {
    SharedBuffer toReceive;
    SharedBuffer toSend;

    if (argc < 3) {
        std::cerr << "gameaccess <host> <port>" << std::endl;
        return 0;
    }

    Utils::printAsciiGameHeader();

    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    Socket server;

    server.connect(host.c_str(), port);

    TextMessage textmessage("");
    std::string dataFromServer = textmessage.receiveFrom(server).getMessage();

    Message message;
    message.deserialize(dataFromServer);
    int clientId = MessageFactory::getClientId(message);

    GameAccess gameAccess(server, host, port);
    gameAccess.setClientId(clientId);
    ListenerAccess listener(server, gameAccess);
    SenderAccess sender(server, toSend);

    gameAccess.start();

    time_t start = time(0);
    // wait for 1 second, while the window is setted
    while (difftime(time(0), start) < 0.5) {}

    listener.start();
    // sends the initial requests, and inmediatly finishes him
    sender.start();

    std::string mapsRequest = MessageFactory::getExistingMapsRequest(gameAccess.getClientId());
    toSend.addData(mapsRequest);

    std::string matchesRequest = MessageFactory::getExistingMatchesRequest(gameAccess.getClientId());
    toSend.addData(matchesRequest);

    toSend.setClientProcessEnded(true);
    sender.join();

    listener.join();
    gameAccess.join();

    server.shutdown();
    server.close();

    return 0;
}
