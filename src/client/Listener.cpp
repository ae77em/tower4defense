#include "Listener.h"
#include "../common/TextMessage.h"

Listener::Listener(Socket &s, SharedBuffer &b) : server(s), buffer(b) { }

Listener::~Listener() {}

void Listener::run(){
    try {
        std::string dataFromServer;
        TextMessage message("");

        while (true) {
            dataFromServer = message.receiveFrom(server).getMessage();
            buffer.addData(dataFromServer);
        }
    } catch (std::exception) {
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}