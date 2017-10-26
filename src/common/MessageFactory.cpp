#include "MessageFactory.h"
#include "Message.h"
#include "Constants.h"
#include "../common/Point.h"

std::string MessageFactory::getPutTowerRequest(int x, int y) {
    Json::Value root;
    Json::Value operation(Json::ValueType::intValue);
    Json::Value xCoord(Json::ValueType::intValue);
    Json::Value yCoord(Json::ValueType::intValue);

    operation = CLIENT_REQUEST_PUT_TOWER;
    xCoord = x;
    yCoord = y;

    root.append(operation);
    root.append(xCoord);
    root.append(yCoord);

    Message message;
    message.setData(root);

    return message.serialize();
}

std::string MessageFactory::getPutTowerNotification() {
    return std::__cxx11::string();
}
