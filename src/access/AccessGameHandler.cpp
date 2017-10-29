#include "AccessGameHandler.h"

GameWindowHandler::GameWindowHandler(const std::string &h, const uint16_t &p) : host(h), port(p){}

GameWindowHandler::~GameWindowHandler(){}

void GameWindowHandler::run() {
    std::string command("./client.exe ");

    command.append(host);
    command.append(" ");
    command.append(std::to_string(port));

    system(command.c_str());
}
