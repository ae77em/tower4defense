#include <iostream>
#include "MessageFactory.h"
#include "Message.h"
#include "Protocol.h"
#include "../common/Point.h"

int MessageFactory::getOperation(Json::Value &root){
    return root[OPERATION_KEY].asInt();
}

std::string MessageFactory::getClientIdNotification(int clientId){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_CREATE_ID;
    root["clientId"] = clientId;

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
    root["clientId"] = clientId;

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
    root["clientId"] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getMapsRequest(int clientId){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_NEW_MATCH;
    root["clientId"] = clientId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getMapsNotification(Json::Value &root) {
    std::string toReturn;
    Message message;

    Json::Value responseRoot(root);

    responseRoot[OPERATION_KEY] = SERVER_NOTIFICATION_NEW_MATCH;

    message.setData(responseRoot);
    toReturn = message.serialize();

    return toReturn;
}


std::string MessageFactory::getCreateMatchRequest(int clientId, int mapId){
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = CLIENT_REQUEST_NEW_MATCH;
    root["clientId"] = clientId;
    root["mapId"] = mapId;

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

std::string MessageFactory::getCreateFullMatchNotification(int gameId, int clientId) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_FULL_NEW_MATCH;

    root["clientId"] = clientId;
    root["gameId"] = gameId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string MessageFactory::getAddPlayerToMatchNotification(int gameId, int clientIdWasAdded) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_ENTER_EXISTING_GAME;

    root["clientId"] = clientIdWasAdded;
    root["gameId"] = gameId;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

//VER COMO MANEJAR ESTO, SI SE HACE EN DOS PASOS O EN UNO
std::string MessageFactory::getAddPlayerAndRunMatchNotification(int gameID, int clientId) {
    return std::__cxx11::string();
}
