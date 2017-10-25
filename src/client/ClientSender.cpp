#include <TextMessage.h>
#include "ClientSender.h"

ClientSender::ClientSender(std::string h, uint16_t p, IntermediateBuffer &b)
        : host(h), port(p), buffer(b) { }

ClientSender::~ClientSender(){}

void ClientSender::run(){
    try {
        std::string dataToSend;
        server.connect(host.c_str(), port);

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
