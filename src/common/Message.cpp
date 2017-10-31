#include "Message.h"
#include "Protocol.h"

#include <cstring>
#include <iostream>

Message::Message() {
    cleanup();
}

Message::~Message() {
}

void Message::cleanup() {
    this->data.clear();
}

bool Message::deserialize(std::string messageData) {
    Json::Reader reader;
    std::string jsonString;
    std::string errs;
    bool parseOk;

    // copy data in dest string
    jsonString.assign(messageData.data(), messageData.length());

    parseOk = reader.parse(jsonString.c_str(), jsonString.c_str() + messageData.length(), data, true);

    return parseOk;
}

bool Message::deserialize(char *messageData) {
    return deserialize(std::string(messageData));
}

std::string Message::serialize() {
    uint32_t messageLenght;
    Json::StyledWriter writer;
    std::string jsonString;
    std::string serializedMessage;

    jsonString = writer.write(getData());
    messageLenght = jsonString.length();

    char *buffer = new char[messageLenght];

    // load the message
    memcpy(buffer, jsonString.c_str(), messageLenght);

    serializedMessage.assign(buffer, messageLenght);
    delete[] buffer;

    return serializedMessage;
}

std::string Message::getJsonString() {
    Json::StyledWriter writer;
    return writer.write(data);
}

void Message::print() {
    std::cout << toString();
}

std::string Message::toString() {
    std::stringstream ss;

    ss << std::endl << this->getData()
       << std::endl;

    return ss.str();
}

void Message::setData(Json::Value &data) {
    this->data = data;
}

Json::Value & Message::getData(){
    return this->data;
}
