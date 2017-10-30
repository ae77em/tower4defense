#include "Server.h"
#include "../common/Socket.h"
#include "../common/TextMessage.h"
#include "../common/Message.h"
#include "../common/MessageFactory.h"
#include "../sdl/Constants.h"
#include "../common/Protocol.h"
#include <iostream>

Server::Server(std::mutex& m,ThreadedQueue<Message>& tq):mutexPlayers(m),
                                                queueMessagesClient(tq){}

Server::~Server() {}

unsigned int Server::getAmountGames(){
    return games.size();
}

void Server::createGameAndNotifyAll(int clientId){
    mutexPlayers.lock();

    int gameId = createGame();

    ServerPlayer * sp = players[clientId];

    sp->setGameId(gameId);

    addPlayerToGame(gameId,sp);

    std::string message = MessageFactory::getCreateMatchNotification( gameId, clientId );
    notifyAll(message);

    mutexPlayers.unlock();
}

void Server::addPlayerToGame(int clientId,int gameId) {
    mutexPlayers.lock();

    ServerGame* serverGame = games.at(gameId);
    ServerPlayer * serverPlayer = players[clientId];

    //chequeo si no se lleno antes
    if(serverGame->isTherePlace()){
        //obtengo el jugador, le seteo el gameID y lo uno a la partida
        serverPlayer->setGameId(gameId);
        addPlayerToGame(gameId,serverPlayer);

        //chequeo si la partida tiene 4 jugadores, de ser asi, arranca la partida
        if(serverGame->isFull()){
            std::string message = MessageFactory::getAddPlayerAndRunMatchNotification( gameId, clientId );
            serverPlayer->sendData(message);
        }else{
            //si no esta llena simplemente notifico a todos el ingreso del jugador
            std::string message = MessageFactory::getAddPlayerToMatchNotification( gameId, clientId );
            notifyAll(message);
        }
    } else{
        //notifico al cliente que se lleno la partida, llegaste tarde pibe jajaja
        std::string message = MessageFactory::getCreateFullMatchNotification( gameId, clientId );
        serverPlayer->sendData(message);
    }

    mutexPlayers.unlock();
}
void Server::notifyAll(std::string message) {
    for(unsigned int i=0; i < players.size(); i++){
        players[i]->sendData(message);
    }
}

void Server::addPlayerToGame(int idGame,ServerPlayer* sp){
    games.at(idGame)->addPlayer(sp);
}


//crea el juego y retorna el id del mismo, EL ID ES EL INDICE DENTRO DEL VECTOR
unsigned int Server::createGame(){
    games.push_back(new ServerGame);

    return games.size() - 1;
}

void Server::createAndRunPlayer(Socket* s){
    ClientRequestHandler *crh = new ClientRequestHandler(*s,queueMessagesClient);
    ServerPlayer* serverPlayer = new ServerPlayer(crh,s->getSocket());

    mutexPlayers.lock();

    players[s->getSocket()] = serverPlayer;
    serverPlayer->launchRequesHandler();

    mutexPlayers.unlock();
}

std::string Server::getGamesList(){
    return "games";
}

void Server::sendGamesListToClient(int clientId){
    std::string games = getGamesList();
    std::string message = MessageFactory::getGamesNotification( clientId, games );

    mutexPlayers.lock();

    players[clientId]->sendData(message);

    mutexPlayers.unlock();
}

void Server::run() {
    std::cout << "GameServer: Corriendo, esperando la conexion del algun cliente"<<std::endl;

    try{
        while ( !queueMessagesClient.isAtEnd() ) {
            std::cout << "GameServer: Esperando que algun cliente mande algo"<<std::endl;

            Message message = queueMessagesClient.pop();
            Json::Value &root = message.getData();

            int op = root[OPERATION_KEY].asInt();

            std::cout << "GameServer: un cliente envio este mensaje: " + root.toStyledString() << std::endl;

            switch(op){
                /* non-gaming requests: */
                case CLIENT_REQUEST_ACCESS_GAME_MENU:{
                    int clientId = root["clientId"].asInt();
                    sendGamesListToClient(clientId);
                    break;
                }
                case CLIENT_REQUEST_NEW_MATCH:{
                    int clientId = root["clientId"].asInt();
                    createGameAndNotifyAll(clientId);
                    break;
                }
                    /* this responses are individual */
                case CLIENT_REQUEST_ENTER_EXISTING_MATCH:{
                    int clientId = root["clientId"].asInt();
                    int gameId = root["gameId"].asInt();

                    addPlayerToGame(clientId,gameId);
                    break;
                }
                case CLIENT_REQUEST_GET_MAPS:{
                   // response = request;
                    break;
                }

                    /* gaming requests: */
                case CLIENT_REQUEST_PUT_TOWER:{
                    //response = MessageFactory::getPutTowerNotification(root);
                    break;
                }
                case CLIENT_REQUEST_MARK_TILE:{
                    //response = MessageFactory::getMarkTileNotification(root);
                    break;
                }
             }
        }
    } catch (std::exception) {
        std::cout << "GameServer: se rompi cola compartida"<<std::endl;
    }

    //std::cout << "CRH: cliente se conecto y mando; "+ data +"y se murio jajaja"<<std::endl;
    std::cout << "GameServer: Se murio, se tendria que haber apagado todo"<<std::endl;
}

void Server::notifyAllWithoutLock(std::string message){
    TextMessage msg(message);
    /*for (auto &client : clients) {
        client.get().push(msg);
    }*/
}
