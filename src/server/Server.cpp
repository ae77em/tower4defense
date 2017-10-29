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

//crea el juego y retorna el id del mismo, EL ID ES EL INDICE DENTRO DEL VECTOR
unsigned int Server::CreateGame(){
    games.push_back(new ServerGame);

    return games.size() - 1;
}

void Server::createAndRunPlayer(Socket s){
    ClientRequestHandler *rp = new ClientRequestHandler(s,queueMessagesClient);
    ServerPlayer* serverPlayer = new ServerPlayer(rp,s.getSocket());

    mutexPlayers.lock();

    players[s.getSocket()] = serverPlayer;
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
            int clientId = root["clientId"].asInt();

            std::cout << "GameServer: un cliente envio este mensaje: " + root.toStyledString() << std::endl;

            switch(op){
                /* non-gaming requests: */
                case CLIENT_REQUEST_ACCESS_GAME_MENU:{
                    sendGamesListToClient(clientId);
                    break;
                }
                case CLIENT_REQUEST_NEW_MATCH:{
                    createGame(clientId);
                    break;
                }
                    /* this responses are individual */
                case CLIENT_REQUEST_ENTER_EXISTING_MATCH:{
                    //response = request;
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


void Server::notifyAll(std::string message) {
    m.lock();
    /*TextMessage msg(message);
    for (auto &client : clients) {
        client.get().push(msg);
    }*/
    m.unlock();
}


void Server::processAndNotify(std::string request){
    m.lock();

    Message message;
    std::string response;

    message.deserialize(request);
    Json::Value &root = message.getData();

    int op = root[OPERATION_KEY].asInt();

    switch(op){
        /* non-gaming requests: */
        case CLIENT_REQUEST_ACCESS_GAME_MENU:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_NEW_MATCH:{
            response = request;
            break;
        }
        /* this responses are individual */
        case CLIENT_REQUEST_ENTER_EXISTING_MATCH:{
            response = request;
            break;
        }
        case CLIENT_REQUEST_GET_MAPS:{
            response = request;
            break;
        }

        /* gaming requests: */
        case CLIENT_REQUEST_PUT_TOWER:{
            response = MessageFactory::getPutTowerNotification(root);
            break;
        }
        case CLIENT_REQUEST_MARK_TILE:{
            response = MessageFactory::getMarkTileNotification(root);
            break;
        }
        default:
            response = request;
    }

    notifyAllWithoutLock(response);

    m.unlock();
}

void Server::notifyAllWithoutLock(std::string message){
    TextMessage msg(message);
    /*for (auto &client : clients) {
        client.get().push(msg);
    }*/
}