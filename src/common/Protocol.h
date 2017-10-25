#ifndef TP4_TOWERDEFENSE_PROTOCOL_H
#define TP4_TOWERDEFENSE_PROTOCOL_H

#include <cstddef>
#include <string>

enum ClientRequestsNonPlaying {
    ACCESS_GAME_MENU = 0,
    ACCESS_CONFIGURATION_MENU = 1,
    CREATE_NEW_GAME = 2,
    ENTER_EXISTING_GAME = 3
};

enum ClientRequestsPlaying {
    PUT_TOWER = 100
};

enum ServerNotifications {
    TOWER_PUTTED = 200
};

class Protocol {
public:
    Protocol() {}
    virtual ~Protocol() {}

    static std::string notificationToString(int notificationType);
    static std::string requestToString(int requestType);
};



#endif
