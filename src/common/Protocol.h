#ifndef TP4_TOWERDEFENSE_PROTOCOL_H
#define TP4_TOWERDEFENSE_PROTOCOL_H

#include <cstddef>
#include <string>

/* Códigos de de requests para acciones previas al juego,
     * i.e. selección de juego, acceso a configuración, etc.
     * */
enum ClientRequestsNonPlaying {
    CLIENT_REQUEST_ACCESS_GAME_MENU = 0,
    CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU = 1,
    CLIENT_REQUEST_CREATE_NEW_GAME = 2,
    CLIENT_REQUEST_ENTER_EXISTING_GAME = 3
};

/* Códigos de de requests para acciones del juego propiamente dicho,
 * i.e. poner torre, marcar lugar...y no sé si hay otra.
 * */
enum ClientRequestsPlaying {
    CLIENT_REQUEST_PUT_TOWER = 100
};

enum ServerNotifications {
    SERVER_NOTIFICATION_PUT_TOWER = 200
};

class Protocol {
public:
    Protocol() {}
    virtual ~Protocol() {}

    static std::string notificationToString(int notificationType);
    static std::string requestToString(int requestType);
};

#endif
