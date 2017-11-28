#include <iostream>
#include <string>
#include "Sender.h"
#include "TextMessage.h"

Sender::Sender(Socket *s, SharedBuffer &b) : server(s), buffer(b) {}

Sender::~Sender(){}

void Sender::shutdown() {}

void Sender::run(){
    try {
        std::string dataToSend;

        while (buffer.isProcessingYet()) {
            dataToSend = buffer.getNextData();
            TextMessage message(dataToSend);
            message.sendTo(*server);
        }

    /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept */
    } catch (std::exception) {}
}
