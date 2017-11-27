#include "Server.h"
#include "MessageFactory.h"
#include "Protocol.h"
#include "Request.h"
#include "model/Map.h"
#include <utility>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <sys/types.h>
#include <dirent.h>


Server::Server(std::mutex &m, ThreadedQueue<Message> &tq)
        : mutexPlayers(m), queueMessagesClient(tq) {}


unsigned int Server::getAmountGames() {
    return matches.size();
}

void
Server::createGame(int clientId, std::string matchName, std::string mapName) {

    bool wasCreated = createMatch(matchName, mapName);

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

    ServerMatch *serverGame = matches.at(mName);
    ServerPlayer *serverPlayer = players.at(clientId);

    //chequeo si no se lleno antes
    if (serverGame->elementsAreAvailables(elements)) {
        //obtengo el jugador, le seteo el gameID y lo uno a la partida
        serverPlayer->setGameId(mName);
        serverPlayer->setStatus(JOINED);
        serverPlayer->setElements(elements);
        serverGame->addPlayer(serverPlayer);

        //si la partida cuanta con elementos disponibles, de lo contrario arranca
        if (!serverGame->isElementsAvailables()) {
            serverGame->setPlaying(true);
            std::string mapName = serverGame->getMapName();

            model::Map mapa = maps.at(mapName);
            std::string serializedMap = mapa.serialize();

            std::string message = MessageFactory::getStartMatchNotification(
                    mName, serializedMap);
            serverGame->notifyAll(message);

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

void Server::addPlayerToMatch(std::string nameMatch, ServerPlayer *sp) {
    matches.at(nameMatch)->addPlayer(sp);
}

// Crea el juego y lo agrega a la lista de partidas en curso
bool Server::createMatch(std::string nameMatch, std::string mapName) {
    model::Map aMap = maps[mapName];

    // Si ya existe un juego con el mismo nombre
    if (matches.find(nameMatch) != matches.end()) return false;

    matches.insert(std::pair<std::string, ServerMatch*>(
            nameMatch,
            new ServerMatch(mutexPlayers, aMap)));
    return true;
}

void Server::createAndRunPlayer(Socket *s) {
    ClientRequestHandler *crh = new ClientRequestHandler(s,
                                                         queueMessagesClient);
    ServerPlayer *serverPlayer = new ServerPlayer(crh, s->getId());

    mutexPlayers.lock();

    int key = s->getId();
    players.insert(std::pair<unsigned int, ServerPlayer *>(key, serverPlayer));
    serverPlayer->launchRequesHandler();

    mutexPlayers.unlock();
}

void Server::notifyTo(int clientId, std::string &message) {
    mutexPlayers.lock();
    players[clientId]->sendData(message);
    mutexPlayers.unlock();
}

void Server::run() {
    // La cola se cierra desde el thread aceptador (Listener)
    // cuando el servidor se apaga
    while (!queueMessagesClient.isAtEnd()) {
        Message msg = queueMessagesClient.pop();
        Request request(msg);
        int op = request.getAsInt(OPERATION_KEY);
        switch (op) {
            // Los bloques case estan encerrados en llaves para limitar
            // el scope de las declaraciones

            /* non-gaming requests: */
            case CLIENT_REQUEST_NEW_MATCH: {
                int clientId = request.getAsInt("clientId");
                std::string nameMatch = request.getAsString("matchName");
                std::string mapName = request.getAsString("mapName");

                createGame(clientId, nameMatch, mapName);
                break;
            } case CLIENT_REQUEST_ENTER_MATCH: {
                int clientId = request.getAsInt("clientId");
                std::string nameMatch = request.getAsString("matchName");
                std::list<std::string> elements =
                        request.getAsStringVector("elements");

                    addPlayerToGame(clientId, nameMatch, elements);
                    break;
                }
                case CLIENT_REQUEST_LEAVE_MATCH: {
                    int clientId = request.getAsInt(CLIENT_ID_KEY);
                    removePlayerFromMatch(clientId);
                    std::string response =
                            MessageFactory::getLeaveMatchNotification();
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_GET_ALL_MAPS: {
                    int clientId = request.getAsInt("clientId");
                    std::vector<std::string> mapsNames = getAllMapsNames();
                   std::string response = MessageFactory::getExistingMapsNotification(mapsNames);
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_GET_ALL_MATCHES: {
                    int clientId = request.getAsInt("clientId");
                    std::vector<std::string> matchNames = getMatchesNames();
                   std::string response = MessageFactory::getExistingMatchesNotification(
                            matchNames);
                    notifyTo(clientId, response);
                    break;
                }
                case CLIENT_REQUEST_GET_UNAVAILABLE_ELEMENTS: {
                    int clientId = request.getAsInt("clientId");
                    std::string nameMatch = request.getAsString("matchName");
                    sendUnavailableElementsToClient(clientId, nameMatch);
                    break;

               } case CLIENT_REQUEST_START_MATCH: {
                    int clientId = request.getAsInt("clientId");
                    std::string matchName = request.getAsString("matchName");

                startMatch(clientId, matchName);
                break;

            /* gaming requests: */
            } case CLIENT_REQUEST_PUT_TOWER: {
                std::string matchName = request.getAsString("matchName");
                int towerType = request.getAsInt("towerType");
                int x = request.getAsInt(XCOORD_KEY);
                int y = request.getAsInt(YCOORD_KEY);

                putTower(matchName, towerType, x, y);
                break;
            } case CLIENT_REQUEST_MARK_TILE: {
                std::string matchName = request.getAsString(MATCH_NAME_KEY);
                int x = request.getAsInt(XCOORD_KEY);
                int y = request.getAsInt(YCOORD_KEY);

                markTile(matchName, x, y);
                break;
            } case CLIENT_REQUEST_CAST_SPELL: {
                std::string matchName = request.getAsString(MATCH_NAME_KEY);
                int x = request.getAsInt(XCOORD_KEY);
                int y = request.getAsInt(YCOORD_KEY);

                castSpell(matchName, x, y);
                break;
            } case CLIENT_REQUEST_TOWER_INFO: {
                int clientId = request.getAsInt(CLIENT_ID_KEY);
                std::string matchName = request.getAsString(MATCH_NAME_KEY);
                int towerId = request.getAsInt("towerId");

                towerInfo(clientId, matchName, towerId);
                break;
            } case CLIENT_REQUEST_UPGRADE_TOWER: {
                std::string matchName = request.getAsString(MATCH_NAME_KEY);
                int clientId = request.getAsInt("clientId");
                int towerId = request.getAsInt("towerId");
                int upgradeType = request.getAsInt("upgradeType");

                upgradeTower(matchName, clientId, towerId, upgradeType);
                break;
            } case SERVER_NOTIFICATION_END_CLIENT_CONNECTION: {
                removeClient(request.getAsInt("clientId"));
                break;

            } default: {
                std::string response = "No se reconoce codigo de operación "
                    + std::to_string(op);
                notifyTo(request.getAsInt("clientId"), response);
            }
        }
    }
}


void Server::startMatch(int clientId, std::string matchName) {
    ServerMatch *serverGame = matches.at(matchName);

    if (serverGame->isPlaying()) {
        //reboto pedido
        ServerPlayer *serverPlayer = players.at(clientId);

        std::string message =
              MessageFactory::getMatchStartedNotification("El match ya inicio");
        serverPlayer->sendData(message);
    } else {
        serverGame->setPlaying(true);

        std::string mapName = serverGame->getMapName();

        model::Map mapa = maps.at(mapName);

        std::string serializedMap = mapa.serialize();

        std::string message = MessageFactory::getStartMatchNotification(
                matchName, serializedMap);
        serverGame->notifyAll(message);

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

    for (std::map<std::string, ServerMatch *>::iterator it = matches.begin();
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

        ServerMatch *sg = matches.at(gameId);

        sg->removePlayer(id);

        if (sg->getAmountPlayers() == 0){
            std::cout << "Server: limpiando partida: "
                      << gameId
                      << "aguardo salida...espero que no sea eterna"
                      << std::endl;

            sg->kill();
            matches.erase(gameId);
        }

    } else if (sp->getStatus() == JOINED) {
        std::string gameId = sp->getGameId();

        ServerMatch *sg = matches.at(gameId);

        sg->enableElements(id);
        sg->removePlayer(id);

        std::cout << "se saco jugador que estaba unido con id: " << id
                  << std::endl;
    }
    sp->kill();

    delete sp;
    players.erase(id);

    mutexPlayers.unlock();
}

void Server::markTile(std::string matchName, int x, int y) {
    ServerMatch *serverGame = matches.at(matchName);
    serverGame->markTile(x, y);
}

void Server::putTower(std::string matchName, int typeOfTower, int x, int y){
    ServerMatch *serverGame = matches.at(matchName);
    serverGame->putTower(typeOfTower, x, y);
}

void Server::castSpell(std::string matchName, int x, int y) {
    ServerMatch *serverGame = matches.at(matchName);
    serverGame->castSpell(x, y);
}

void Server::upgradeTower(std::string matchName,
                          int clientId,
                          int towerId,
                          int upgradeType) {
    ServerMatch *serverGame = matches.at(matchName);
    serverGame->upgradeTower(clientId, towerId, upgradeType);
}

void Server::towerInfo(int clientId, std::string matchName, int towerId) {
    ServerMatch *serverGame = matches.at(matchName);
    serverGame->towerInfo(clientId, towerId);
}

void Server::loadMaps() {
    DIR* dirp = opendir("resources/maps/");
    std::string mapFilename;
    std::string mapName;
    unsigned long nameLenght = 10;
    struct dirent *dp;

    while ((dp = readdir(dirp)) != NULL) {
        if (std::string(dp->d_name).find(".json") != std::string::npos){
            mapFilename.assign("resources/maps/");
            mapFilename.append(dp->d_name);

            model::Map aMap = model::Map::loadFromFile(mapFilename);
            mapName = std::string(dp->d_name);
            nameLenght = mapName.find('.');
            mapName = mapName.substr(0, nameLenght);
            aMap.setName(mapName);

            std::pair<std::string, model::Map>
                    mapPair(aMap.getName(),std::move(aMap));
            maps.insert(mapPair);
        }
    }

    closedir(dirp);
}

std::vector<std::string> Server::getAllMapsNames() {
    std::vector<std::string> toReturn;
    std::string namePosta;

    for (auto it=maps.begin(); it!=maps.end(); ++it){
        toReturn.push_back(it->first);
    }

    return toReturn;
}

void Server::removePlayerFromMatch(int id) {

    mutexPlayers.lock();

    ServerPlayer *sp = players.at(id);

    std::string gameId = sp->getGameId();

    ServerMatch *sg = matches.at(gameId);

    sg->enableElements(id);
    sg->removePlayer(id);

    std::cout << "se saco jugador que estaba unido con id: " << id
              << std::endl;

    mutexPlayers.unlock();
}
