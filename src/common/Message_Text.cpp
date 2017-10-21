#include "Message_Text.h"
#include <stdexcept>

// snprintf
#ifndef _XOPEN_SOURCE
    #define _XOPEN_SOURCE 500
#endif
#include <stdio.h>

TextMessage::TextMessage(std::string message) : message(message) {}

void TextMessage::sendThrough(Socket &sock) {
    /* Supposing the message's length fits in four bytes, the lenght's decimal
     * representation fits in ten bytes, not including the terminating
     * null byte
     */
    char prefix[11];
    snprintf(prefix, 11, "%010lu", message.size());

    sock.send(prefix, 10);
    sock.send(message.c_str(), message.size());
}

TextMessage receiveFrom(Socket &sock) {
    char prefix[11];
    if (sock.receive(prefix, 10) < 10)
        throw std::runtime_error("No se pudo recibir el prefijo completo.");
    prefix[10] = '\0';

    size_t len;
    sscanf(prefix, "%010lu", &len);

    // Don't forget the all-important terminating null byte.
    char *message = new char[len + 1];
    if (sock.receive(message, len) < (int)len) {
        delete message;
        throw std::runtime_error("No se pudo recibir la cola completa.");
    }

    std::string s(message);
    return TextMessage(message);
}

const std::string& TextMessage::getMessage() {
    return message;
}