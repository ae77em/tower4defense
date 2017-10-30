#include <iostream>
#include "Game.h"
#include "Listener.h"
#include "Sender.h"
#include "../common/TextMessage.h"
#include "../common/Message.h"

int main(int argc, char *argv[]) {
    std::string host = std::string(argv[1]);
    uint16_t port = atoi(argv[2]);

    SharedBuffer in;
    SharedBuffer out;

    Socket client;

    client.connect(host.c_str(), port);

    char me[5];

    client.receive(me,4);

    me[4] = 0;

    std::cout<<"mensaje server: "<<me<<std::endl;

    /*Game game(in, out, client.getSocket());

    Listener listener(client, in);
    Sender sender(client, out);

    game.start();
    listener.start();
    sender.start();

    game.join();
    listener.join();
    sender.join();

    client.shutdown();
    client.close();*/

    return 0;
}

