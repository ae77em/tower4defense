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

