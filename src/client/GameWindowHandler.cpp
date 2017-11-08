#include "GameWindowHandler.h"
#include "../common/Socket.h"
#include "../common/SharedBuffer.h"
#include "Game.h"
#include "Listener.h"
#include "Sender.h"
#include "../common/Utils.h"

GameWindowHandler::GameWindowHandler(const std::string &h, const uint16_t &p) : host(h), port(p){}

GameWindowHandler::~GameWindowHandler(){}

void GameWindowHandler::run(Socket &server, int clientId) {
    SharedBuffer in;
    SharedBuffer out;

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

    /*server.shutdown();
    server.close();*/
}
