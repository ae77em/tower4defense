#include <TextMessage.h>
#include "ClientListener.h"

ClientListener::ClientListener(std::string h, uint16_t p, IntermediateBuffer &b)
        : host(h), port(p), buffer(b) { }

ClientListener::~ClientListener() {}

void ClientListener::run(){
    try {
        std::string dataFromServer;
        server.connect(host.c_str(), port);
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