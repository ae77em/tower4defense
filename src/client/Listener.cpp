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
                   SharedBuffer &bfr/*,
                   SharedBuffer &other*/)
        : server(s),
          notificable(ga),
          buffer(bfr)/*,
          buffer2(other)*/ { }

Listener::~Listener() {}

void Listener::run(){
    try {
        std::string dataFromServer;
        TextMessage textMessage("");

        while (true) {
            dataFromServer = textMessage.receiveFrom(*server).getMessage();

            Message message;
            std::string response = "";

            message.deserialize(dataFromServer);

            int op = MessageFactory::getOperation(message);

            switch (op){
                /*
                 * ACCESS OPERATIONS
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
                 * GAME OPERATIONS
                 * Todas aquellas operaciones que no corresponden con algo que se hace en
                 * el login, lo consideramos acción de juego, por ende agregamos la acción al
                 * buffer compartido con el thread donde se desarrolla el mismo. No se si es
                 * la mejor solución, pero es la que me surgió primero, teniendo en cuenta que
                 * las cosas las tenía resueltas por separado...
                 */
                /*case SERVER_NOTIFICATION_PUT_TOWER: // ver como sacarlo de
                    // este grupo
                case SERVER_NOTIFICATION_MARK_TILE:
                case SERVER_NOTIFICATION_CAST_SPELL:
                case SERVER_NOTIFICATION_TOWER_INFO:
                case SERVER_NOTIFICATION_APPLY_UPGRADE: {
                    buffer2.addData(dataFromServer);
                    break;
                }*/
                default:
                    buffer.addData(dataFromServer);
            }

            std::cout << response << "dataFromServer: "
                      << dataFromServer << std::endl;
        }
    } catch (std::exception) {
        std::cout << "se cierra el listener del cliente del juego" << std::endl;
        /* Catcheo la excepción que se lanza cuando fuerzo la salida del accept
         * que queda sin recibir ningún request. No hago nada...
         */
    }
}
