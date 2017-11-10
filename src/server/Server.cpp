#include "Server.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"
#include "Request.h"

Server::Server(std::mutex &m, ThreadedQueue<Message> &tq) : mutexPlayers(m),
                                                            queueMessagesClient(tq) {}


unsigned int Server::getAmountGames() {
    return games.size();
}

void Server::createGame(int clientId, string matchName) {
    bool wasCreated = createMatch(matchName);

    std::string message = "";

    if (wasCreated) {
        message = MessageFactory::getNewMatchNotification(matchName);
    } else {
        message = MessageFactory::getNewMatchErrorNotification(matchName, "La partida ya existe");
    }

    notifyAll(message);
}

void Server::addPlayerToGame(int clientId, std::string mName, vector<string> elements) {
    mutexPlayers.lock();

    ServerGame *serverGame = games.at(mName);
    ServerPlayer *serverPlayer = players.at(clientId);

    //chequeo si no se lleno antes
    if (serverGame->elementsAreAvailables(elements)) {
        //obtengo el jugador, le seteo el gameID y lo uno a la partida
        serverPlayer->setGameId(mName);
        serverPlayer->setElements(elements);
        serverGame->addPlayer(serverPlayer);

        //chequeo si la partida tiene 4 jugadores, de ser asi, arranca la partida
        if (serverGame->isFull()) {
            std::string message = MessageFactory::getAddPlayerAndRunMatchNotification(mName, clientId);
            serverGame->notifyAll(message);

            //VEDR DESPUES COMO SINCRONIZAR PARA QUE NO ARRANQUE MUY ANTES
            serverGame->startGame();
        } else {
            //si no esta llena simplemente notifico a todos el ingreso del jugador
            std::string message = MessageFactory::getAddPlayerToMatchNotification(mName, clientId, elements);
            notifyPlayerAdded(clientId, message);
        }
    } else {
        //notifico al cliente que se lleno la partida
        std::string message = MessageFactory::getMatchNotAvailableNotification(mName, "Elemento no disponible");
        serverPlayer->sendData(message);
    }

    mutexPlayers.unlock();
}

void Server::notifyAll(std::string message) {
    std::cout << "Notificando a: " << players.size() << " jugadores" << std::endl;

    for (std::pair<unsigned int, ServerPlayer *> player : players) {
        player.second->sendData(message);
    }
}

void Server::notifyAllExpeptTo(int clientId, std::string message) {
    std::cout << "Notificando a: " << players.size() - 1 << " jugadores" << std::endl;

    for (std::pair<unsigned int, ServerPlayer *> player : players) {
        if ((int) player.first != clientId) {
            player.second->sendData(message);
        }
    }
}

void Server::addPlayerToMatch(std::string nameMatch, ServerPlayer *sp) {
    games.at(nameMatch)->addPlayer(sp);
}

//crea el juego y retorna el id del mismo, el id es el nombre del match...
bool Server::createMatch(std::string nameMatch) {
    if( games.find(nameMatch) == games.end() ){
        games.insert(std::pair<std::string, ServerGame *>(nameMatch, new ServerGame(mutexPlayers)));
        return true;
    }
    return false;
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

            Message messageRequest = queueMessagesClient.pop();
            Request request(messageRequest);

            int op = request.getAsInt(OPERATION_KEY);

            std::cout << "GameServer: un cliente envio este mensaje: " + messageRequest.toString() << std::endl;
            std::string response;

            switch (op) {
                /* non-gaming requests: */
                case CLIENT_REQUEST_NEW_MATCH: {
                    //FALTA TOMAR EL VALOR DEL MAPA
                    int clientId = request.getAsInt("clientId");
                    std::string nameMatch = request.getAsString("matchName");

                    createGame(clientId, nameMatch);
                    break;
                }
                /* this responses are individual */
                case CLIENT_REQUEST_ENTER_MATCH: {
                    int clientId = request.getAsInt("clientId");
                    std::string nameMatch = request.getAsString("matchName");
                    std::vector<std::string> elements = request.getAsStringVector("elements");

                    addPlayerToGame(clientId,nameMatch,elements);

                    break;
                }
                case CLIENT_REQUEST_GET_ALL_MAPS: {
                    int clientId = MessageFactory::getClientId(messageRequest);
                    response = MessageFactory::getExistingMapsNotification();
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_GET_ALL_MATCHES: {
                    int clientId = request.getAsInt("clientId");
                    std::vector<std::string> matchNames = getMatchesNames();
                    response = MessageFactory::getExistingMatchesNotification(matchNames);
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_GET_UNAVAILABLE_ELEMENTS: {
                    int clientId = request.getAsInt("clientId");
                    std::string nameMatch = request.getAsString("matchName");
                    sendUnavailableElementsToClient(clientId, nameMatch);
                    break;
                }
                case CLIENT_REQUEST_ENTER_EXISTING_MATCH: {
                    int clientId = request.getAsInt("clientId");
                    std::string matchName = request.getAsString("matchName");
                    std::vector<std::string> elements = request.getAsStringVector("elements");

                    addPlayerToGame(clientId, matchName, elements);
                    break;
                }
                case CLIENT_REQUEST_START_MATCH: {
                    int clientId = request.getAsInt("clientId");
                    std::string matchName = request.getAsString("matchName");

                    startMatch(clientId, matchName);
                    break;
                }

                /* gaming requests: */
                case CLIENT_REQUEST_PUT_TOWER: {
                    response = MessageFactory::getPutTowerNotification(messageRequest);
                    notifyAll(response);
                    break;
                }
                case CLIENT_REQUEST_MARK_TILE: {
                    response = MessageFactory::getMarkTileNotification(messageRequest);
                    notifyAll(response);
                    break;
                }
                case SERVER_NOTIFICATION_END_CLIENT_CONNECTION:
                    std::cout
                            << "el cliente "
                            << std::to_string(MessageFactory::getClientId(messageRequest))
                            << " se desconectó."
                            << std::endl;

                    removeClient(MessageFactory::getClientId(messageRequest));
                    break;
                default: {
                    int clientId = MessageFactory::getClientId(messageRequest);
                    response = "No se reconoce codigo de operación ";
                    response.append(std::to_string(op));
                    response.append("\n");
                    response.append(messageRequest.getJsonString());
                    notifyTo(clientId, response);
                }
            }
        }
    } catch (std::exception) {
        std::cout << "GameServer: se rompi cola compartida" << std::endl;
    }

    std::cout << "GameServer: Se murio, se tendria que haber apagado todo" << std::endl;
}

void Server::removeClient(int id) {
    players.erase(id);
}

void Server::startMatch(int clientId, string matchName) {
    ServerGame *serverGame = games.at(matchName);

    if (serverGame->isPlaying()) {
        //reboto pedido
        ServerPlayer *serverPlayer = players.at(clientId);

        std::string message = MessageFactory::getMatchStartedNotification("El match ya inicio");
        serverPlayer->sendData(message);
    } else {
        serverGame->setPlaying(true);

        std::string message = MessageFactory::getStartMatchNotification(matchName);
        serverGame->notifyAll(message);

        //VEDR DESPUES COMO SINCRONIZAR PARA QUE NO ARRANQUE MUY ANTES
        serverGame->startGame();
    }
}

void Server::sendUnavailableElementsToClient(int clientId, string matchName) {
    std::list<std::string> elements = games.at(matchName)->getUnavailableElements();

    std::string message = MessageFactory::getUnavailableElementsNotification(elements);

    notifyTo(clientId, message);
}

std::vector<std::string> Server::getMatchesNames() {
    std::vector<std::string> matchesNames;

    for (std::map<std::string, ServerGame *>::iterator it = games.begin(); it != games.end(); ++it) {
        matchesNames.push_back(it->first);
    }

    return matchesNames;
}

void Server::notifyPlayerAdded(int clientId, string message) {
    std::cout << "Notificando a: " << players.size() << " jugadores" << std::endl;

    for (std::pair<unsigned int, ServerPlayer *> player : players) {
        player.second->sendData(message);
    }

}
