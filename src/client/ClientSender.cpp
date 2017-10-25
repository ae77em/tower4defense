#include "ClientSender.h"
#include "../common/TextMessage.h"

ClientSender::ClientSender(Socket &s, IntermediateBuffer &b) : server(s), buffer(b) {}

ClientSender::~ClientSender(){}

void ClientSender::run(){
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
