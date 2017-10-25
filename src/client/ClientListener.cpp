#include "ClientListener.h"
#include "../common/TextMessage.h"

ClientListener::ClientListener(Socket &s, IntermediateBuffer &b) : server(s), buffer(b) { }

ClientListener::~ClientListener() {}

void ClientListener::run(){
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