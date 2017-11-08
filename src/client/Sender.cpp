#include <iostream>
#include "Sender.h"
#include "../common/TextMessage.h"

Sender::Sender(Socket &s, SharedBuffer &b) : server(s), buffer(b) {}

Sender::~Sender(){}

void Sender::shutdown() {
    //server.shutdown();
}

void Sender::run(){
    try {
        std::string dataToSend;

        while (buffer.isProcessingYet()) {
            dataToSend = buffer.getNextData();
            TextMessage message(dataToSend);
            message.sendTo(server);
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}
