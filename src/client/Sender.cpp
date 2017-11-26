#include <iostream>
#include <string>
#include "Sender.h"
#include "TextMessage.h"

Sender::Sender(Socket *s, SharedBuffer &b) : server(s), buffer(b) {}

Sender::~Sender(){}

void Sender::shutdown() {}

void Sender::run(){
    //TODO: reducir el scope del try-catch
    try {
        std::string dataToSend;

        while (buffer.isProcessingYet()) {
            dataToSend = buffer.getNextData();
            std::cout << "data que para enviar: " << dataToSend << std::endl;
            TextMessage message(dataToSend);
            message.sendTo(*server);
        }

    /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept */
    } catch (std::exception) {}
}
