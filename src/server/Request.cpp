#include "Request.h"
#include "../common/MessageFactory.h"

Request::Request(Message& m):requestMessage(m){
}

//refactorizar el trato con el json
std::string Request::getAsString(std::string key){
    Json::Value &root = requestMessage.getData();

    return root[key].asString();
}

int Request::getAsInt(std::string key){
    Json::Value &root = requestMessage.getData();

    return root[key].asInt();
}