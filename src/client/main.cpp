#include "Game.h"
#include "Listener.h"
#include "Sender.h"
#include "../common/Utils.h"

int main(int argc, char *argv[]) {
    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    SharedBuffer in;
    SharedBuffer out;

    Socket client;

    client.connect(host.c_str(), port);

    Game game(in, out, client.getSocket());

    Listener listener(client, in);
    Sender sender(client, out);

    Utils::printAsciiGameHeader();

    game.start();
    listener.start();
    sender.start();

    game.join();
    listener.join();
    sender.join();

    client.shutdown();
    client.close();

    return 0;
}

