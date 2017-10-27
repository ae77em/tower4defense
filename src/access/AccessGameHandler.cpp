#include "AccessGameHandler.h"

GameWindowHandler::GameWindowHandler(){}

GameWindowHandler::~GameWindowHandler(){}

void GameWindowHandler::run() {
    system("./client.exe localhost 7777");
}
