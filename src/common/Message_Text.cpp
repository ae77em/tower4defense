#include "../common/Message_Text.h"

// snprintf
#ifndef _XOPEN_SOURCE
    #define _XOPEN_SOURCE 500
#endif
#include <stdio.h>

TextMessage::TextMessage(std::string &&message) : message(std::move(message)) {}

void TextMessage::sendThrough(Socket &sock) {
    /* Supposing the message's length fits in four bytes, the lenght's decimal
     * representation fits in ten bytes, not including the terminating
     * null byte
     */
    char prefix[11];
    snprintf(prefix, 11, "%010ld", message.size());

    sock.send(prefix, 10);
    sock.send(message.c_str(), message.size());
}

void TextMessage::receiveFrom(Socket &sock) {
    char prefix[11];
    sock.receive(prefix, 10);
    prefix[10] = '\0';

    size_t len;
    sscanf(prefix, "%010ld", &len);

    // Don't forget the all-important terminating null byte.
    char message[len + 1];
    sock.receive(message, len);
    this->message = message;
}

