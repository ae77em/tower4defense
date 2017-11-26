#include "Protocol.h"
#include <string>

std::string Protocol::notificationToString(int notificationType) {
    std::string toReturn = "GameNotificationFactory";

    switch (notificationType) {
        case CLIENT_REQUEST_ACCESS_GAME_MENU:
            toReturn.append(" Access GamePlayWindow Menu");
            break;
        case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU:
            toReturn.append(" Access Configuration Menu");
            break;
        case CLIENT_REQUEST_NEW_MATCH:
            toReturn.append(" Create new GamePlayWindow");
            break;
        default:
            toReturn.append(" Unknown");
            break;
    }

    return toReturn;
}

std::string Protocol::requestToString(int requestType) {
    std::string toReturn = "Request";

    switch (requestType) {
        case CLIENT_REQUEST_ACCESS_GAME_MENU:
            toReturn.append(" to Access GamePlayWindow");
            break;
        case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU:
            toReturn.append(" to Access Configuration");
            break;
        case CLIENT_REQUEST_NEW_MATCH:
            toReturn.append(" to Create new GamePlayWindow");
            break;
        case CLIENT_REQUEST_PUT_TOWER:
            toReturn.append(" to Put a Tower");
            break;
        default:
            toReturn.append(" Unknown");
            break;
    }

    return toReturn;
}


