#include <iostream>
#include "MessageFactory.h"
#include "Message.h"
#include "Protocol.h"
#include "../common/Point.h"

int MessageFactory::getOperation(Message& request){
    Json::Value &root = request.getData();
    return root[OPERATION_KEY].asInt();
}

std::vector<std::string> MessageFactory::getMaps(Message& message){
    std::vector<std::string> response;
    Json::Value &root = message.getData();
    for (Json::Value &map : root["maps"]){
        response.push_back(map.asString());
    }
    return response;
}

std::string MessageFactory::getMapName(Message& message){
    std::string response;
    Json::Value &root = message.getData();

    response = root.get("mapName", "").asString();

    return response;
}

std::string MessageFactory::getMatchName(Message& message){
    std::string response;
    Json::Value &root = message.getData();

    response = root.get("matchName", "").asString();

    return response;
}

std::string MessageFactory::getClientIdNotification(int clientId){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_CLIENT_ID;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getCreateMatchNotification(int gameId,int clientIdWhoCreatedGame){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;
    //por el momento id el cliente con su ID, deberia de tener un nombre
    //sino que se  va a mostar en los combos
    root["clientIdWhoCreatedGame"] = clientIdWhoCreatedGame;
    root["mgameIdapId"] = gameId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}


std::string MessageFactory::getGamesNotification(int clientId,std::string games){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_MATCHES;
    root["games"] = games;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}



/* ****************************************************************************
 * NON-GAMING REQUESTS
 * ****************************************************************************
 * */
std::string MessageFactory::getGamesRequest(int clientId){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_MATCHES;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getExistingMapsRequest(int clientId){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_MAPS;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getExistingMapsNotification() {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(Json::objectValue);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_GET_MAPS;
    responseRoot["maps"] = Json::arrayValue;
    /* TODO: obtener la data posta... */
    responseRoot["maps"].append("mapa1");
    responseRoot["maps"].append("mapa2");

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getExistingMatchesRequest(int clientId) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_MATCHES;
    root[CLIENT_ID_KEY] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getExistingMatchesNotification(std::set<std::string> &matches) {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(Json::objectValue);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_GET_MATCHES;
    responseRoot["matches"] = Json::arrayValue;

    for (const std::string &match : matches) {
        responseRoot["matches"].append(match);
    }

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getCreateMatchRequest(int clientId, std::string mapName){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_NEW_MATCH;
    root[CLIENT_ID_KEY] = clientId;
    root["mapName"] = mapName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

/* ****************************************************************************
 * GAMING REQUESTS
 * ****************************************************************************
 * */
std::string MessageFactory::getPutTowerRequest(int clientId, int x, int y, bool isPut) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_PUT_TOWER;
    root[CLIENT_ID_KEY] = clientId;
    root["xCoord"] = x;
    root["yCoord"] = y;
    root["isPut"] = isPut;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getPutTowerNotification(Message &request) {
    Json::Value &root = request.getData();
    std::string toReturn;
    Message message;

    Json::Value responseRoot(root);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_PUT_TOWER;

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getMarkTileRequest(int clientId, int x, int y, bool isPut) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_MARK_TILE;
    root[CLIENT_ID_KEY] = clientId;
    root["xCoord"] = x;
    root["yCoord"] = y;
    root["isMark"] = isPut;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getMarkTileNotification(Message &request) {
    Json::Value &root = request.getData();
    std::string toReturn;
    Message message;

    Json::Value responseRoot(root);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_MARK_TILE;

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getNewMatchRequest(int clientId, std::string &mapName, std::string &matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_NEW_MATCH;
    root[CLIENT_ID_KEY] = clientId;
    root["mapName"] = mapName;
    root["matchName"] = matchName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getNewMatchNotification(std::string matchName) {
    Json::Value responseRoot;
    Message message;

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;
    responseRoot["matchName"] = matchName;

    message.setData(responseRoot);

    return message.serialize();
}

std::string MessageFactory::getNewMatchErrorNotification(std::string matchName,
                                                         std::string errorMessage) {
    Json::Value responseRoot;
    Message message;

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;
    responseRoot["matchName"] = matchName;
    responseRoot["errorMessage"] = errorMessage;

    message.setData(responseRoot);

    return message.serialize();
}

std::string MessageFactory::getMatchNotAvailableNotification(std::string matchName, std::string errorMessage) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_MATCH_NOT_AVAILABLE;

    root["errorMessage"] = errorMessage;
    root["matchName"] = matchName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string
MessageFactory::getAddPlayerToMatchNotification(std::string matchName,
                                                int clientIdWasAdded,
                                                std::vector<std::string> elements) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_ENTER_EXISTING_GAME;

    root[CLIENT_ID_KEY] = clientIdWasAdded;
    root["matchName"] = matchName;
    for (std::string element : elements){
        root["elements"].append(element);
    }

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

//VER COMO MANEJAR ESTO, SI SE HACE EN DOS PASOS O EN UNO
std::string MessageFactory::getAddPlayerAndRunMatchNotification(std::string gameID, int clientId) {
    return std::__cxx11::string();
}

int MessageFactory::getClientId(Message &request) {
    Json::Value &root = request.getData();
    int clientId = root[CLIENT_ID_KEY].asInt();
    return clientId;
}

int MessageFactory::getMatchId(Message &request) {
    Json::Value &root = request.getData();
    int matchId = root["matchId"].asInt();
    return matchId;
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

std::string MessageFactory::getMovementNotification(int enemyId, int moveable, int x, int y, int direction) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_MOVE_ENEMY;
    root["enemyId"] = enemyId;
    root["xCoord"] = x;
    root["yCoord"] = y;
    root["typeOfMoveable"] = moveable;
    root["direction"] = direction;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

Point MessageFactory::getPoint(Message message) {
    Json::Value &root = message.getData();
    int x = root.get("xCoord","-1").asInt();
    int y = root.get("yCoord","-1").asInt();
    return Point(x,y);
}

int MessageFactory::getDirection(Message message) {
    Json::Value &root = message.getData();
    int direction = root.get("direction","0").asInt();
    return direction;
}

int MessageFactory::getEnemyId(Message message) {
    Json::Value &root = message.getData();
    int direction = root.get("enemyId","-1").asInt();
    return direction;
}

std::string MessageFactory::getStartMatchRequest(int clientId, std::string &matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_NEW_MATCH;
    root[CLIENT_ID_KEY] = clientId;
    root["matchName"] = matchName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}


std::string MessageFactory::getStartMatchNotification(std::string matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_START_MATCH;
    root["matchName"] = matchName;

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

std::string MessageFactory::getMatchElementsNotification(std::list<std::string> elements) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_GET_ELEMENTS;
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
    for (Json::Value &map : root["elements"]){
        response.push_back(map.asString());
    }
    return response;
}

std::string MessageFactory::getMatchElementsRequest(int clientId, std::string matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_ELEMENTS;
    root["clientId"] = clientId;
    root["matchName"] = matchName;

    message.setData(root);

    return message.serialize();
}

std::string MessageFactory::getEnterMatchRequest(int clientId, std::string matchName,
                                                 std::vector<std::string> elements) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_ENTER_MATCH;
    root["clientId"] = clientId;
    root["matchName"] = matchName;
    for (std::string element : elements){
        root["elements"].append(element);
    }

    message.setData(root);

    return message.serialize();
}
