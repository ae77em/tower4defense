#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Listener.h"
#include "../common/TextMessage.h"
#include "../common/SharedBuffer.h"
#include "../common/MessageFactory.h"
#include "../common/Message.h"
#include "../common/Protocol.h"
#include "Notificable.h"

Listener::Listener(Socket *s,
                   Notificable &ga,
                   SharedBuffer &bfr)
        : server(s),
          notificable(ga),
          sharedBuffer(bfr) { }

Listener::~Listener() {}

void Listener::run(){
    std::string dataFromServer;
    TextMessage textMessage("");

    while (true) {
        try {
            dataFromServer = textMessage.receiveFrom(*server).getMessage();
        } catch (std::exception) {
            std::cout
                    << "se cierra el listener del cliente del juego"
                    << std::endl;
            break;
        }

        Message message;
        std::string response = "";

        message.deserialize(dataFromServer);

        int op = MessageFactory::getOperation(message);

        switch (op){
            /*
             * Operaciones sobre ventana de acceso:
             * Operaciones que modifican la interfaz, y requieren que se
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
            /*
             * Operaciones de juego.
             */
            default:
                sharedBuffer.addData(dataFromServer);
        }

        std::cout << response << "dataFromServer: "
                  << dataFromServer << std::endl;
    }
}
