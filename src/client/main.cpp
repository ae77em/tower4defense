#include "Game.h"
#include "Listener.h"
#include "Sender.h"
#include "../common/Utils.h"
#include "../common/TextMessage.h"
#include "../common/Message.h"
#include "../common/MessageFactory.h"

int main(int argc, char *argv[]) {
    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    SharedBuffer in;
    SharedBuffer out;

    Socket server;

    server.connect(host.c_str(), port);

    TextMessage textmessage("");
    std::string dataFromServer = textmessage.receiveFrom(server).getMessage();

    Message message;

    message.deserialize(dataFromServer);

    int clientId = MessageFactory::getClientId(message);

    Game game(in, out, clientId);

    Listener listener(server, in);
    Sender sender(server, out);

    Utils::printAsciiGameHeader();

    game.start();
    listener.start();
    sender.start();

    game.join();
    listener.join();
    sender.join();

    server.shutdown();
    server.close();

    return 0;
}

