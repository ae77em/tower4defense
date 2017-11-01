#include "Server.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"

Server::Server(std::mutex &m, ThreadedQueue<Message> &tq) : mutexPlayers(m),
                                                            queueMessagesClient(tq) {}

Server::~Server() {}

unsigned int Server::getAmountGames() {
    return games.size();
}

void Server::createGameAndNotifyAll(Message &request) {
    mutexPlayers.lock();

    int clientId = MessageFactory::getClientId(request);
    int gameId = createMatch();

    ServerPlayer *sp = players.at(clientId);

    sp->setGameId(gameId);

    addPlayerToMatch(gameId, sp);

    std::string message = "bleh...";//MessageFactory::getNewMatchNotification(gameId, clientId);

    notifyAll(message);

    mutexPlayers.unlock();
}

void Server::addPlayerToGame(int clientId, int gameId) {
    mutexPlayers.lock();

    ServerGame *serverGame = games.at(gameId);
    ServerPlayer *serverPlayer = players[clientId];

    //chequeo si no se lleno antes
    if (serverGame->isTherePlace()) {
        //obtengo el jugador, le seteo el gameID y lo uno a la partida
        serverPlayer->setGameId(gameId);
        addPlayerToMatch(gameId, serverPlayer);

        //chequeo si la partida tiene 4 jugadores, de ser asi, arranca la partida
        if (serverGame->isFull()) {
            std::string message = MessageFactory::getAddPlayerAndRunMatchNotification(gameId, clientId);
            serverPlayer->sendData(message);
        } else {
            //si no esta llena simplemente notifico a todos el ingreso del jugador
            std::string message = MessageFactory::getAddPlayerToMatchNotification(gameId, clientId);
            notifyAll(message);
        }
    } else {
        //notifico al cliente que se lleno la partida, llegaste tarde pibe jajaja
        std::string message = MessageFactory::getCreateFullMatchNotification(gameId, clientId);
        serverPlayer->sendData(message);
    }

    mutexPlayers.unlock();
}

void Server::notifyAll(std::string message) {
    std::cout<<"Notificando a: "<<players.size() << " jugadores"<<std::endl;

    for (std::pair<unsigned int, ServerPlayer *> player : players){
        player.second->sendData(message);
    }
}

void Server::addPlayerToMatch(int idMatch, ServerPlayer *sp) {
    games.at(idMatch)->addPlayer(sp);
}

//crea el juego y retorna el id del mismo, EL ID ES EL INDICE DENTRO DEL VECTOR
unsigned int Server::createMatch() {
    games.push_back(new ServerGame);

    return games.size() - 1;
}

void Server::createAndRunPlayer(Socket *s) {
    ClientRequestHandler *crh = new ClientRequestHandler(s, queueMessagesClient);
    ServerPlayer *serverPlayer = new ServerPlayer(crh, s->getSocket());

    mutexPlayers.lock();

    unsigned int key = s->getSocket();
    players.insert(std::pair<unsigned int, ServerPlayer *>(key, serverPlayer));
    serverPlayer->launchRequesHandler();

    mutexPlayers.unlock();
}

std::string Server::getGamesList() {
    return "games";
}

void Server::sendGamesListToClient(int clientId) {
    std::string games = getGamesList();
    std::string message = MessageFactory::getGamesNotification(clientId, games);

    notifyTo(clientId, message);
}

void Server::notifyTo(int clientId, std::string &message) {
    mutexPlayers.lock();
    players[clientId]->sendData(message);
    mutexPlayers.unlock();
}


void Server::run() {
    std::cout << "GameServer: Corriendo, esperando la conexion del algun cliente" << std::endl;

    try {
        while (!queueMessagesClient.isAtEnd()) {
            std::cout << "GameServer: Esperando que algun cliente mande algo" << std::endl;

            Message request = queueMessagesClient.pop();

            int op = MessageFactory::getOperation(request);

            std::cout << "GameServer: un cliente envio este mensaje: " + request.toString() << std::endl;
            std::string response;

            switch (op) {
                /* non-gaming requests: */
                case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU: {
                    response = request.getJsonString();
                    break;
                }
                case CLIENT_REQUEST_ACCESS_GAME_MENU: {
                    int clientId = MessageFactory::getClientId(request);
                    sendGamesListToClient(clientId);
                    break;
                }
                case CLIENT_REQUEST_NEW_MATCH: {
                    //createGameAndNotifyAll(request);
                    response = MessageFactory::getNewMatchNotification(request,matches);
                    notifyAll(response);
                    break;
                }
                /* this responses are individual */
                case CLIENT_REQUEST_ENTER_MATCH: {
                    response = request.getJsonString();
                    break;
                }
                case CLIENT_REQUEST_GET_MAPS: {
                    int clientId = MessageFactory::getClientId(request);
                    response = MessageFactory::getExistingMapsNotification();
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_GET_MATCHES: {
                    int clientId = MessageFactory::getClientId(request);
                    response = MessageFactory::getExistingMatchesNotification(matches);
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_ENTER_EXISTING_MATCH: {
                    int clientId = MessageFactory::getClientId(request);
                    int gameId = MessageFactory::getMatchId(request);
                    addPlayerToGame(clientId, gameId);
                    break;
                }
                /* gaming requests: */
                case CLIENT_REQUEST_PUT_TOWER: {
                    response = MessageFactory::getPutTowerNotification(request);
                    notifyAll(response);
                    break;
                }
                case CLIENT_REQUEST_MARK_TILE: {
                    response = MessageFactory::getMarkTileNotification(request);
                    notifyAll(response);
                    break;
                }
                case SERVER_NOTIFICATION_END_CLIENT_CONNECTION:
                    std::cout
                        << "el cliente "
                        << std::to_string(MessageFactory::getClientId(request))
                        << " se desconectó."
                        << std::endl;

                    removeClient(MessageFactory::getClientId(request));
                    break;
                default: {
                    int clientId = MessageFactory::getClientId(request);
                    response = "No se reconoce codigo de operación ";
                    response.append(std::to_string(op));
                    response.append("\n");
                    response.append(request.getJsonString());
                    notifyTo(clientId, response);
                }
            }
        }
    } catch (std::exception) {
        std::cout << "GameServer: se rompi cola compartida" << std::endl;
    }

    //std::cout << "CRH: cliente se conecto y mando; "+ data +"y se murio jajaja"<<std::endl;
    std::cout << "GameServer: Se murio, se tendria que haber apagado todo" << std::endl;
}

void Server::notifyAllWithoutLock(std::string message) {
    TextMessage msg(message);
    /*for (auto &client : clients) {
        client.get().push(msg);
    }*/
}

void Server::removeClient(int id) {
    players.erase(id);
}
