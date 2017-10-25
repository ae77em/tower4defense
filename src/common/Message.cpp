#include "Message.h"
#include <netdb.h>
#include <cstring>
#include <string>

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
    message_lenght_t messageLength;
    Json::Reader reader;
    std::string jsonString;
    std::string errs;
    bool parseOk;

    // get the message length (the first 32 bits)
    memcpy(&messageLength, messageData.data(), sizeof(message_lenght_t));

    // decode message length
    messageLength = ntohl(messageLength);

    // copy data in dest string
    jsonString.assign(messageData.data() + sizeof(message_lenght_t), messageLength);

    parseOk = reader.parse(jsonString.c_str(), jsonString.c_str() + messageLength, data, true);

    return parseOk;
}

bool Message::deserialize(char *messageData) {
    return deserialize(std::string(messageData));
}

std::string Message::serialize() {
    message_lenght_t messageLenght, messageLenghtBigEndian;
    Json::StyledWriter writer;
    std::string jsonString;
    std::string serializedMessage;

    jsonString = writer.write(getData());
    messageLenght = jsonString.length();

    int totalLength = sizeof(message_lenght_t) + messageLenght;

    messageLenghtBigEndian = htonl(messageLenght);

    char *buffer = new char[totalLength];

    // load the message length
    memcpy(buffer, &messageLenghtBigEndian, sizeof(message_lenght_t));
    // load the message
    memcpy(buffer + sizeof(message_lenght_t), jsonString.c_str(), messageLenght);

    serializedMessage.assign(buffer, totalLength);
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

    ss << std::endl << "Json: "
       << std::endl << this->getData()
       << std::endl;

    return ss.str();
}

void Message::setData(Json::Value &data) {
    this->data = data;
}

Json::Value &Message::getData() {
    return this->data;
}
