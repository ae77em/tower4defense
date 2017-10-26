#include "Game.h"
#include "Listener.h"
#include "Sender.h"

int main(int argc, char *argv[]) {

    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    SharedBuffer in;
    SharedBuffer out;

    Game game(in, out);

    Socket server;

    server.connect(host.c_str(), port);

    Listener listener(server, in);
    Sender sender(server, out);

    game.start();
    listener.start();
    sender.start();

    game.join();
    listener.join();
    sender.join();

    return 0;
}

