#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Listener.h"
#include "TextMessage.h"
#include "SharedBuffer.h"
#include "MessageFactory.h"
#include "Message.h"
#include "Protocol.h"
#include "Notificable.h"

Listener::Listener(Socket *s, Notificable &ga, SharedBuffer &bfr)
        : server(s), notificable(ga), buffer(bfr) {}

Listener::~Listener() {}

void Listener::run(){
    std::string dataFromServer;
    TextMessage textMessage("");
    bool ended;

    while (!ended) {
        try {
            dataFromServer = textMessage.receiveFrom(*server).getMessage();
        } catch (std::exception) {
            dataFromServer = MessageFactory::getEndConectionNotification();
            buffer.setClientProcessEnded(true);
            ended = true;
        }

        Message message;
        std::string response = "";

        message.deserialize(dataFromServer);

        int op = MessageFactory::getOperation(message);

        switch (op){
            /* Operaciones que modifican la interfaz, y requieren que se
             * use el Glib::dispatcher
             */
            case SERVER_NOTIFICATION_START_MATCH:
            case SERVER_NOTIFICATION_CLIENT_ID:
            case SERVER_NOTIFICATION_GET_ALL_MAPS:
            case SERVER_NOTIFICATION_GET_ALL_MATCHES:
            case SERVER_NOTIFICATION_NEW_MATCH:
            case SERVER_NOTIFICATION_ENTER_EXISTING_MATCH:
            case SERVER_NOTIFICATION_GET_UNAVAILABLE_ELEMENTS: {
                notificable.notify(dataFromServer);
                break;
            }
            /* Las operaciones que no son de login son acciones de juego,
             * y las agregamos al buffer compartido del mismo.
             */
            default:
                buffer.addData(dataFromServer);
        }
    }
}
