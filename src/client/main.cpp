#include "Game.h"
#include "ClientListener.h"
#include "ClientSender.h"

int main(int argc, char *argv[]) {

    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    IntermediateBuffer in;
    IntermediateBuffer out;

    Game game(in, out);

    Socket server;

    server.connect(host.c_str(), port);

    ClientListener listener(server, in);
    ClientSender sender(server, out);

    game.start();
    listener.start();
    sender.start();

    game.join();
    listener.join();
    sender.join();

    return 0;
}

