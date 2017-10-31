#ifndef TP4_TOWERDEFENSE_PROTOCOL_H
#define TP4_TOWERDEFENSE_PROTOCOL_H

#include <cstddef>
#include <string>

/* Códigos de de requests para acciones previas al juego,
     * i.e. selección de juego, acceso a configuración, etc.
     * */
enum ClientRequestsNonPlaying {
    CLIENT_REQUEST_END_GAME = 0,
    CLIENT_REQUEST_ACCESS_GAME_MENU = 1,
    CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU = 2,
    CLIENT_REQUEST_NEW_MATCH = 3,
    CLIENT_REQUEST_ENTER_EXISTING_MATCH = 4,
    CLIENT_REQUEST_GET_MAPS = 5,
    CLIENT_REQUEST_GET_MATCHES = 6,
    CLIENT_REQUEST_ENTER_MATCH = 7
};

/* Códigos de de requests para acciones del juego propiamente dicho,
 * i.e. poner torre, marcar lugar...y no sé si hay otra.
 * */
enum ClientRequestsPlaying {
    CLIENT_REQUEST_PUT_TOWER = 100,
    CLIENT_REQUEST_MARK_TILE = 101
};

enum ServerNonPlayingNotifications {
    SERVER_NOTIFICATION_ACCESS_GAME_MENU = 200,
    SERVER_NOTIFICATION_ACCESS_GAME_MENU_ERROR = 299,  // 299 - 0
    SERVER_NOTIFICATION_ACCESS_CONFIGURATION_MENU = 201,
    SERVER_NOTIFICATION_NEW_MATCH = 202,
    SERVER_NOTIFICATION_NEW_MATCH_ERROR = 297,  // 299 - 2
    SERVER_NOTIFICATION_ENTER_EXISTING_GAME = 203,
    SERVER_NOTIFICATION_GET_MAPS = 204,
    SERVER_NOTIFICATION_GET_MATCHES = 205,
    SERVER_NOTIFICATION_MATCH_FULL = 206,
    SERVER_NOTIFICATION_CLIENT_ID = 207
};

enum ServerPlayingNotifications {
    SERVER_NOTIFICATION_PUT_TOWER = 300,
    SERVER_NOTIFICATION_MARK_TILE = 301
};


class Protocol {
public:
    Protocol() {}
    virtual ~Protocol() {}

    static std::string notificationToString(int notificationType);
    static std::string requestToString(int requestType);
};

#endif
