#include "Request.h"
#include "../common/MessageFactory.h"

Request::Request(Message& m):requestMessage(m){
}

std::string Request::getAsString(std::string key){
    Json::Value &root = requestMessage.getData();

    return root[key].asString();
}

int Request::getAsInt(std::string key){
    Json::Value &root = requestMessage.getData();

    return root[key].asInt();
}

std::list<std::string> Request::getAsStringVector(std::string key){
    Json::Value &root = requestMessage.getData();
    std::list<std::string> elements;

    for (Json::Value &map : root[key]){
        elements.push_back(map.asString());
    }

    return elements;
}
