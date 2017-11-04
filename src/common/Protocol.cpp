#include "Protocol.h"
#include <string>

std::string Protocol::notificationToString(int notificationType) {
    std::string toReturn = "GameNotification";

    switch (notificationType) {
        case CLIENT_REQUEST_ACCESS_GAME_MENU:
            toReturn.append(" Access Game Menu");
            break;
        case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU:
            toReturn.append(" Access Configuration Menu");
            break;
        case CLIENT_REQUEST_NEW_MATCH:
            toReturn.append(" Create new Game");
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
            toReturn.append(" to Access Game");
            break;
        case CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU:
            toReturn.append(" to Access Configuration");
            break;
        case CLIENT_REQUEST_NEW_MATCH:
            toReturn.append(" to Create new Game");
            break;
        case CLIENT_REQUEST_ENTER_EXISTING_MATCH:
            toReturn.append(" to Enter in exisiting Game");
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


