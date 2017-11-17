#include "Server.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"
#include "../common/Request.h"
#include <utility>
#include <map>
#include <vector>
#include <string>
#include <list>

Server::Server(std::mutex &m, ThreadedQueue<Message> &tq)
        : mutexPlayers(m), queueMessagesClient(tq) {}


unsigned int Server::getAmountGames() {
    return matches.size();
}

void Server::createGame(int clientId, std::string matchName) {
    bool wasCreated = createMatch(matchName);

    std::string message = "";

    if (wasCreated) {
        message = MessageFactory::getNewMatchNotification(clientId, matchName);
    } else {
        message =
                MessageFactory::getNewMatchErrorNotification(clientId,
                                                        matchName,
                                                        "La partida ya existe");
    }

    notifyAll(message);
}

void Server::addPlayerToGame(int clientId, std::string mName,
                             std::list<std::string> elements) {
    mutexPlayers.lock();

    ServerGame *serverGame = matches.at(mName);
    ServerPlayer *serverPlayer = players.at(clientId);

    //chequeo si no se lleno antes
    if (serverGame->elementsAreAvailables(elements)) {
        //obtengo el jugador, le seteo el gameID y lo uno a la partida
        serverPlayer->setGameId(mName);
        serverPlayer->setStatus(JOINED);
        serverPlayer->setElements(elements);
        serverGame->addPlayer(serverPlayer);

        //si la partida tiene 4 jugadores, arranca
        if (serverGame->isFull()) {
            std::string message =
                    MessageFactory::getAddPlayerAndRunMatchNotification(
                    mName, clientId);
            serverGame->notifyAll(message);

            //VEDR DESPUES COMO SINCRONIZAR PARA QUE NO ARRANQUE MUY ANTES
            serverGame->startGame();
        } else {
            //si no esta llena notifico a todos el ingreso del jugador
            std::string message =
                    MessageFactory::getAddPlayerToMatchNotification(mName,
                                                                    clientId,
                                                                    elements);
            notifyPlayerAdded(message);
        }
    } else {
        //notifico al cliente que se lleno la partida
        std::string message = MessageFactory::getMatchNotAvailableNotification(
                mName, "Elemento no disponible");
        serverPlayer->sendData(message);
    }

    mutexPlayers.unlock();
}

void Server::notifyAll(std::string message) {
    std::cout << "Notificando a: " << players.size() << " jugadores"
              << std::endl;

    for (std::pair<unsigned int, ServerPlayer *> player : players) {
        player.second->sendData(message);
    }
}

void Server::notifyAllExpeptTo(int clientId, std::string message) {
    std::cout << "Notificando a: " << players.size() - 1 << " jugadores"
              << std::endl;

    for (std::pair<unsigned int, ServerPlayer *> player : players) {
        if ((int) player.first != clientId) {
            player.second->sendData(message);
        }
    }
}

void Server::addPlayerToMatch(std::string nameMatch, ServerPlayer *sp) {
    matches.at(nameMatch)->addPlayer(sp);
}

//crea el juego y retorna el id del mismo, el id es el nombre del match...
bool Server::createMatch(std::string nameMatch) {
    if (matches.find(nameMatch) == matches.end()) {
        matches.insert(
                std::pair<std::string, ServerGame *>(nameMatch,
                                                     new ServerGame(
                                                             mutexPlayers)));
        return true;
    }
    return false;
}

void Server::createAndRunPlayer(Socket *s) {
    ClientRequestHandler *crh = new ClientRequestHandler(s,
                                                         queueMessagesClient);
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
    std::cout
            << "GameServer: Corriendo, esperando la conexion del algun cliente"
            << std::endl;

    try {
        while (!queueMessagesClient.isAtEnd()) {
            std::cout << "GameServer: Esperando que algun cliente mande algo"
                      << std::endl;

            Message messageRequest = queueMessagesClient.pop();
            Request request(messageRequest);

            int op = request.getAsInt(OPERATION_KEY);

            std::cout << "GameServer: un cliente envio este mensaje: " +
                         messageRequest.toString() << std::endl;
            std::string response;

            switch (op) {
                /* non-gaming requests: */
                case CLIENT_REQUEST_NEW_MATCH: {
                    int clientId = request.getAsInt("clientId");
                    std::string nameMatch = request.getAsString("matchName");

                    createGame(clientId, nameMatch);
                    break;
                }
                /* this responses are individual */
                case CLIENT_REQUEST_ENTER_MATCH: {
                    int clientId = request.getAsInt("clientId");
                    std::string nameMatch = request.getAsString("matchName");
                    std::list<std::string> elements =
                            request.getAsStringVector("elements");

                    addPlayerToGame(clientId, nameMatch, elements);
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
                    response = MessageFactory::getExistingMatchesNotification(
                            matchNames);
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_GET_UNAVAILABLE_ELEMENTS: {
                    int clientId = request.getAsInt("clientId");
                    std::string nameMatch = request.getAsString("matchName");
                    sendUnavailableElementsToClient(clientId, nameMatch);
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
                    std::string matchName = request.getAsString("matchName");
                    int towerType = request.getAsInt("towerType");
                    int x = request.getAsInt(XCOORD_KEY);
                    int y = request.getAsInt(YCOORD_KEY);

                    putTower(matchName, towerType, x, y);
                    break;
                }
                case CLIENT_REQUEST_MARK_TILE: {
                    std::string matchName = request.getAsString(MATCH_NAME_KEY);
                    int x = request.getAsInt(XCOORD_KEY);
                    int y = request.getAsInt(YCOORD_KEY);

                    markTile(matchName, x, y);
                    break;
                }
                case CLIENT_REQUEST_CAST_SPELL: {
                    std::string matchName = request.getAsString(MATCH_NAME_KEY);
                    int x = request.getAsInt(XCOORD_KEY);
                    int y = request.getAsInt(YCOORD_KEY);

                    castSpell(matchName, x, y);
                    break;
                }
                case CLIENT_REQUEST_TOWER_INFO: {
                    int clientId = request.getAsInt(CLIENT_ID_KEY);
                    std::string matchName = request.getAsString(MATCH_NAME_KEY);
                    int towerId = request.getAsInt("towerId");

                    towerInfo(clientId, matchName, towerId);
                    break;
                }
                case CLIENT_REQUEST_UPGRADE_TOWER: {
                    std::string matchName = request.getAsString(MATCH_NAME_KEY);
                    int towerId = request.getAsInt("towerId");
                    int upgradeType = request.getAsInt("upgradeType");

                    upgradeTower(matchName, towerId, upgradeType);
                    break;
                }
                case SERVER_NOTIFICATION_END_CLIENT_CONNECTION: {
                    std::cout
                            << "el cliente "
                            << std::to_string(
                                    MessageFactory::getClientId(messageRequest))
                            << " se desconectó."
                            << std::endl;

                    int id = 0;
                    id = MessageFactory::getClientId(messageRequest);

                    removeClient(id);

                    std::cout << "GameServer: se termino de matar al cliennte "
                              << id << std::endl;
                    std::cout << "Ahora hay " << players.size() << " clientes"
                              << std::endl;

                    break;
                }
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

    std::cout << "GameServer: Se murio, se tendria que haber apagado todo"
              << std::endl;
}


void Server::startMatch(int clientId, std::string matchName) {
    ServerGame *serverGame = matches.at(matchName);

    if (serverGame->isPlaying()) {
        //reboto pedido
        ServerPlayer *serverPlayer = players.at(clientId);

        std::string message = MessageFactory::getMatchStartedNotification(
                "El match ya inicio");
        serverPlayer->sendData(message);
    } else {
        serverGame->setPlaying(true);

        std::string message = MessageFactory::getStartMatchNotification(
                matchName);
        serverGame->notifyAll(message);

        //VEDR DESPUES COMO SINCRONIZAR PARA QUE NO ARRANQUE MUY ANTES
        serverGame->startGame();
    }
}

void Server::sendUnavailableElementsToClient(int clientId,
                                             std::string matchName) {
    std::list<std::string> elements = matches.at(
            matchName)->getUnavailableElements();

    std::string message = MessageFactory::getUnavailableElementsNotification(
            elements);

    notifyTo(clientId, message);
}

std::vector<std::string> Server::getMatchesNames() {
    std::vector<std::string> matchesNames;

    for (std::map<std::string, ServerGame *>::iterator it = matches.begin();
         it != matches.end(); ++it) {
        matchesNames.push_back(it->first);
    }

    return matchesNames;
}

void Server::notifyPlayerAdded(std::string message) {
    std::cout << "Notificando a: " << players.size() << " jugadores"
              << std::endl;

    for (std::pair<unsigned int, ServerPlayer *> player : players) {
        player.second->sendData(message);
    }
}

void Server::removeClient(int id) {
    std::cout << "GameServer: Matando al cliennte " << id << std::endl;

    mutexPlayers.lock();

    ServerPlayer *sp = players.at(id);

    if (sp->getStatus() == PLAYING) {
        std::string gameId = sp->getGameId();

        ServerGame *sg = matches.at(gameId);

        sg->removePlayer(id);

        if (sg->getAmountPlayers() == 0){
            std::cout << "Server: limpiando partida: "
                      << gameId
                      << "aguardo salida...espero que no sea eterna"
                      << std::endl;

            sg->kill();
            //delete sg;
            matches.erase(gameId);
        }

    } else if (sp->getStatus() == JOINED) {
        std::string gameId = sp->getGameId();

        ServerGame *sg = matches.at(gameId);

        sg->enableElements(id);
        sg->removePlayer(id);

        std::cout << "se saco jugador que estaba unido con id: " << id
                  << std::endl;
    } else {
        // VER DESPUES SI HAY ALGO
        // QUE HACER CUANDO EL STATUS DEL CLIENTE ES NOTPLAYING
    }

    sp->kill();

    delete sp;
    players.erase(id);

    mutexPlayers.unlock();
}

void Server::markTile(std::string matchName, int x, int y) {
    ServerGame *serverGame = matches.at(matchName);
    serverGame->markTile(x, y);
}

void Server::putTower(std::string matchName, int typeOfTower, int x, int y){
    ServerGame *serverGame = matches.at(matchName);
    serverGame->putTower(typeOfTower, x, y);
}

void Server::castSpell(std::string matchName, int x, int y) {
    ServerGame *serverGame = matches.at(matchName);
    serverGame->castSpell(x, y);
}

void Server::upgradeTower(std::string matchName, int towerId, int upgradeType) {
    ServerGame *serverGame = matches.at(matchName);
    serverGame->upgradeTower(towerId, upgradeType);
}

void Server::towerInfo(int clientId, std::string matchName, int towerId) {
    ServerGame *serverGame = matches.at(matchName);
    serverGame->towerInfo(clientId, towerId);
}


