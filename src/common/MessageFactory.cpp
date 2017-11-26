#include <iostream>
#include "MessageFactory.h"
#include "Protocol.h"
#include <string>
#include <list>
#include <vector>

int MessageFactory::getOperation(Message &request) {
    Json::Value &root = request.getData();
    return root[OPERATION_KEY].asInt();
}

std::vector<std::string> MessageFactory::getMaps(Message &message) {
    std::vector<std::string> response;
    Json::Value &root = message.getData();
    for (Json::Value &map : root["maps"]) {
        response.push_back(map.asString());
    }
    return response;
}

std::vector<std::string> MessageFactory::getMatches(Message &message) {
    std::vector<std::string> response;
    Json::Value &root = message.getData();
    for (Json::Value &map : root["matches"]) {
        response.push_back(map.asString());
    }
    return response;
}

std::string MessageFactory::getMatchName(Message &message) {
    std::string response;
    Json::Value &root = message.getData();

    response = root.get(MATCH_NAME_KEY, "").asString();

    return response;
}

std::string MessageFactory::getClientIdNotification(int clientId) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_CLIENT_ID;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

/* ****************************************************************************
 * NON-GAMING REQUESTS
 * ****************************************************************************
 * */
std::string MessageFactory::getExistingMapsRequest(int clientId) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_ALL_MAPS;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getExistingMapsNotification(
        std::vector<std::string> mapsNames) {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(Json::objectValue);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_GET_ALL_MAPS;
    responseRoot["maps"] = Json::arrayValue;
    
    for (std::string mapName : mapsNames){
        responseRoot["maps"].append(mapName);
    }

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getExistingMatchesRequest(int clientId) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_ALL_MATCHES;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getExistingMatchesNotification(
        std::vector<std::string> &matches) {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(Json::objectValue);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_GET_ALL_MATCHES;
    responseRoot["matches"] = Json::arrayValue;

    for (const std::string &match : matches) {
        responseRoot["matches"].append(match);
    }

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

/* ****************************************************************************
 * GAMING REQUESTS
 * ****************************************************************************
 * */
std::string
MessageFactory::getPutTowerRequest(std::string matchName, int towerType, int x,
                                   int y) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_PUT_TOWER;
    root[MATCH_NAME_KEY] = matchName;
    root["towerType"] = towerType;
    root[XCOORD_KEY] = x;
    root[YCOORD_KEY] = y;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getPutTowerNotification(int towerType,
                                                    int x,
                                                    int y) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_PUT_TOWER;
    root["towerType"] = towerType;
    root[XCOORD_KEY] = x;
    root[YCOORD_KEY] = y;

    message.setData(root);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getMarkTileRequest(std::string matchName, int x,
                                               int y) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_MARK_TILE;
    root[MATCH_NAME_KEY] = matchName;
    root[XCOORD_KEY] = x;
    root[YCOORD_KEY] = y;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getMarkTileNotification(int x, int y) {
    Json::Value root = Json::objectValue;
    std::string toReturn;
    Message message;

    Json::Value responseRoot(root);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_MARK_TILE;
    responseRoot[XCOORD_KEY] = x;
    responseRoot[YCOORD_KEY] = y;

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getNewMatchRequest(int clientId,
                                               std::string &mapName,
                                               std::string &matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;
    std::string completeMatchName;

    completeMatchName.assign(mapName);
    completeMatchName.append("-");
    completeMatchName.append(matchName);

    root[OPERATION_KEY] = CLIENT_REQUEST_NEW_MATCH;
    root[CLIENT_ID_KEY] = clientId;
    root[MAP_NAME_KEY] = mapName;
    root[MATCH_NAME_KEY] = completeMatchName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string
MessageFactory::getNewMatchNotification(int clientId, std::string matchName) {
    Json::Value responseRoot;
    Message message;

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;
    responseRoot[CLIENT_ID_KEY] = clientId;
    responseRoot[MATCH_NAME_KEY] = matchName;

    message.setData(responseRoot);

    return message.serialize();
}

std::string MessageFactory::getNewMatchErrorNotification(int clientId,
                                                    std::string matchName,
                                                    std::string errorMessage) {
    Json::Value responseRoot;
    Message message;

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;
    responseRoot[MATCH_NAME_KEY] = matchName;
    responseRoot["errorMessage"] = errorMessage;

    message.setData(responseRoot);

    return message.serialize();
}

std::string
MessageFactory::getMatchNotAvailableNotification(std::string matchName,
                                                 std::string errorMessage) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_MATCH_NOT_AVAILABLE;

    root["errorMessage"] = errorMessage;
    root[MATCH_NAME_KEY] = matchName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string
MessageFactory::getAddPlayerToMatchNotification(std::string matchName,
                                            int clientIdWasAdded,
                                            std::list<std::string> elements) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_ENTER_EXISTING_MATCH;

    root[CLIENT_ID_KEY] = clientIdWasAdded;
    root[MATCH_NAME_KEY] = matchName;
    for (std::string element : elements) {
        root["elements"].append(element);
    }

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

//VER COMO MANEJAR ESTO, SI SE HACE EN DOS PASOS O EN UNO
std::string
MessageFactory::getAddPlayerAndRunMatchNotification(std::string gameID,
                                                    int clientId) {
    // TODO VER DE TERMINAR ESTO...SI HAY TIEMPO
    return std::string();
}

int MessageFactory::getClientId(Message &request) {
    Json::Value &root = request.getData();
    int clientId = root[CLIENT_ID_KEY].asInt();
    return clientId;
}

std::string MessageFactory::getClientEndConectionNotification(int clientId) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_END_CLIENT_CONNECTION;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

Point MessageFactory::getPoint(Message message) {
    Json::Value &root = message.getData();
    int x = root.get(XCOORD_KEY, "-1").asInt();
    int y = root.get(YCOORD_KEY, "-1").asInt();
    return Point(x, y);
}

int MessageFactory::getDirection(Message message) {
    Json::Value &root = message.getData();
    int direction = root.get("direction", "0").asInt();
    return direction;
}

int MessageFactory::getEnemyId(Message message) {
    Json::Value &root = message.getData();
    int enemyId = root.get(ENEMY_ID_KEY, "-1").asInt();
    return enemyId;
}

int MessageFactory::getHordeId(Message message) {
    Json::Value &root = message.getData();
    int hordeId = root.get(HORDE_ID_KEY, "-1").asInt();
    return hordeId;
}

std::string
MessageFactory::getStartMatchRequest(int clientId, std::string &matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_START_MATCH;
    root[CLIENT_ID_KEY] = clientId;
    root[MATCH_NAME_KEY] = matchName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}


std::string MessageFactory::getStartMatchNotification(std::string matchName,
                                               std::string serializedMap) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_START_MATCH;
    root[MATCH_NAME_KEY] = matchName;
    root["serializedMap"] = serializedMap;

    message.setData(root);

    return message.serialize();
}

std::string MessageFactory::getMatchStartedNotification(std::string cause) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_STARTED_MATCH;
    root["cause"] = cause;

    message.setData(root);

    return message.serialize();
}

std::string MessageFactory::getUnavailableElementsNotification(
        std::list<std::string> elements) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_GET_UNAVAILABLE_ELEMENTS;
    root["maps"] = Json::arrayValue;

    for (std::string elementName : elements) {
        root["elements"].append(elementName);
    }

    message.setData(root);

    return message.serialize();
}

std::list<std::string> MessageFactory::getElements(Message message) {
    std::list<std::string> response;
    Json::Value &root = message.getData();
    for (Json::Value &map : root["elements"]) {
        response.push_back(map.asString());
    }
    return response;
}

std::string MessageFactory::getUnavailableElementsRequest(int clientId,
                                                      std::string matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_UNAVAILABLE_ELEMENTS;
    root[CLIENT_ID_KEY] = clientId;
    root[MATCH_NAME_KEY] = matchName;

    message.setData(root);

    return message.serialize();
}

std::string
MessageFactory::getEnterMatchRequest(int clientId, std::string matchName,
                                     std::vector<std::string> elements) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_ENTER_MATCH;
    root[CLIENT_ID_KEY] = clientId;
    root[MATCH_NAME_KEY] = matchName;
    for (std::string element : elements) {
        root["elements"].append(element);
    }

    message.setData(root);

    return message.serialize();
}

std::vector<Message> MessageFactory::getMovementNotifications(Message message) {
    std::list<std::string> response;
    Json::Value &root = message.getData();
    std::vector<Message> messagesToReturn;

    for (Json::Value &enemy : root["enemies"]) {
        Message m;
        m.setData(enemy);
        messagesToReturn.push_back(m);
    }

    return messagesToReturn;
}

std::string
MessageFactory::getCastSpellRequest(std::string matchName, int x, int y) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_CAST_SPELL;
    root[MATCH_NAME_KEY] = matchName;
    root[XCOORD_KEY] = x;
    root[YCOORD_KEY] = y;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string
MessageFactory::getTowerInfoRequest(int clientId,
                                    std::string matchName,
                                    int towerId) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_TOWER_INFO;
    root[CLIENT_ID_KEY] = clientId;
    root[MATCH_NAME_KEY] = matchName;
    root[TOWER_ID_KEY] = towerId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getUpgradeRequest(int clientId,
                                              std::string matchName,
                                              int towerId,
                                              int upgradeType) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_UPGRADE_TOWER;
    root[CLIENT_ID_KEY] = clientId;
    root[MATCH_NAME_KEY] = matchName;
    root[TOWER_ID_KEY] = towerId;
    root["upgradeType"] = upgradeType;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getCastSpellNotification(int x, int y) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_CAST_SPELL;
    root[XCOORD_KEY] = x;
    root[YCOORD_KEY] = y;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getSerializedMap(Message message) {
    std::string response;
    Json::Value &root = message.getData();

    response = root.get("serializedMap", "").asString();

    return response;
}

