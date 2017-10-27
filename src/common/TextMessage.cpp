#include "TextMessage.h"
#include <stdexcept>

// snprintf
#ifndef _XOPEN_SOURCE
    #define _XOPEN_SOURCE 500
#endif
#include <stdio.h>
#include <iostream>

TextMessage::TextMessage(std::string message) : message(message) {}

void TextMessage::sendTo(Socket &sock) {
    char prefix[MESSAGE_PREFIX_LENGTH];
    snprintf(prefix, MESSAGE_PREFIX_LENGTH, "%010lu", message.size());

    prefix[MESSAGE_PREFIX_LENGTH-1] = '\0';

    std::cout << "mando length: " << prefix << std::endl;

    sock.send(prefix, MESSAGE_PREFIX_LENGTH);

    std::cout << "mando mensaje: " << message << std::endl;

    sock.send(message.c_str(), message.size());
}

TextMessage TextMessage::receiveFrom(Socket &sock) {
    char prefix[MESSAGE_PREFIX_LENGTH];
    if (sock.receive(prefix, MESSAGE_PREFIX_LENGTH) < MESSAGE_PREFIX_LENGTH)
        throw std::runtime_error("No se pudo recibir el prefijo completo.");

    prefix[MESSAGE_PREFIX_LENGTH-1] = '\0';

    size_t len;
    sscanf(prefix, "%010lu", &len);

    // Don't forget the all-important terminating null byte.
    char *message = new char[len+1];
    if (sock.receive(message, len) < (int)len) {
        delete message;
        throw std::runtime_error("No se pudo recibir la cola completa.");
    }

    message[len] = '\0';

    std::string s(message);

    return TextMessage(s);
}

const std::string& TextMessage::getMessage() {
    return message;
}
