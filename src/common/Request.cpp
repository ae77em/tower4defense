#include "Request.h"
#include "MessageFactory.h"
#include <string>
#include <list>

Request::Request(Message& m):requestMessage(m){ }

std::string Request::getAsString(std::string key){
    Json::Value &root = requestMessage.getData();

    return root[key].asString();
}

int Request::getAsInt(std::string key){
    Json::Value &root = requestMessage.getData();

    return root.get(key, -1).asInt();
}

bool Request::getAsBool(std::string key){
    Json::Value &root = requestMessage.getData();

    return root[key].asBool();
}

double Request::getAsDouble(std::string key){
    Json::Value &root = requestMessage.getData();

    return root[key].asDouble();
}

std::list<std::string> Request::getAsStringVector(std::string key){
    Json::Value &root = requestMessage.getData();
    std::list<std::string> elements;

    for (Json::Value &map : root[key]){
        elements.push_back(map.asString());
    }

    return elements;
}

Point Request::getAsPoint(std::string xKey, std::string yKey) {
    Json::Value &root = requestMessage.getData();

    int x = root[xKey].asInt();
    int y = root[yKey].asInt();

    Point point(x, y);

    return point;
}
