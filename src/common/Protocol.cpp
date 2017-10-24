#include "Protocol.h"
#include <string>

std::string Protocol::notificationToString(int notificationType) {
    std::string toReturn = "Notification";

    switch (notificationType) {
        case ACCESS_GAME_MENU:
            toReturn.append(" Access Game Menu");
            break;
        case ACCESS_CONFIGURATION_MENU:
            toReturn.append(" Access Configuration Menu");
            break;
        case CREATE_NEW_GAME:
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
        case ACCESS_GAME_MENU:
            toReturn.append(" to Access Game");
            break;
        case ACCESS_CONFIGURATION_MENU:
            toReturn.append(" to Access Configuration");
            break;
        case CREATE_NEW_GAME:
            toReturn.append(" to Create new Game");
            break;
        case ENTER_EXISTING_GAME:
            toReturn.append(" to Enter in exisiting Game");
            break;
        case PUT_TOWER:
            toReturn.append(" to Put a Tower");
            break;
        default:
            toReturn.append(" Unknown");
            break;
    }

    return toReturn;
}


