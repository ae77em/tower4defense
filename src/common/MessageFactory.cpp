#include <iostream>
#include "MessageFactory.h"
#include "Message.h"
#include "Protocol.h"
#include "../common/Point.h"

int MessageFactory::getOperation(Json::Value &root){
    return root[OPERATION_KEY].asInt();
}

/* ****************************************************************************
 * NON-GAMING REQUESTS
 * ****************************************************************************
 * */
std::string MessageFactory::getExistingMapsRequest(int clientId){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_GET_MAPS;
    root["clientId"] = clientId;

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
    root["clientId"] = clientId;

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
    root["clientId"] = clientId;
    root["mapName"] = mapName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getCreateMatchNotification(Json::Value &root) {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(root);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;

    message.setData(responseRoot);
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
    root["clientId"] = clientId;
    root["xCoord"] = x;
    root["yCoord"] = y;
    root["isPut"] = isPut;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getPutTowerNotification(Json::Value &root) {
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
    root["clientId"] = clientId;
    root["xCoord"] = x;
    root["yCoord"] = y;
    root["isMark"] = isPut;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getMarkTileNotification(Json::Value &root) {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(root);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_MARK_TILE;

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getNewMatchRequest(std::string &mapName, std::string &matchName) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_NEW_MATCH;
    root["mapName"] = mapName;
    root["matchName"] = matchName;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getNewMatchNotification(Json::Value &root, std::set<std::string> &matches) {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(root);

    std::string mapName = root["mapName"].asString();
    std::string matchName = root["matchName"].asString();

    std::string composedMatchName;
    composedMatchName.assign(mapName);
    composedMatchName.append(" ");
    composedMatchName.append(matchName);

    if (matches.find(matchName) == matches.end()) {
        matches.insert(matchName);
        responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;
        responseRoot["mapName"] = mapName;
        responseRoot["matchName"] = matchName;
    } else {
        responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH_ERROR;
        responseRoot["mapName"] = mapName;
        responseRoot["matchName"] = matchName;
        responseRoot["errorDesc"] = "La partida no se puede crear ya que existe otra con ese nombre.";
    }

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}
