#ifndef TP4_TOWERDEFENSE_MESSAGEFACTORY_H
#define TP4_TOWERDEFENSE_MESSAGEFACTORY_H

static const char *const OPERATION_KEY = "operation";

#include <string>
#include <jsoncpp/json/json.h>

class MessageFactory {
public:
    /*
     * Retorna el request de texto para agregar una torre listo para ser enviado al server.
     * x: coordenada x donde se quiere colocar la torre.
     * y: coordenada y donde se quiere colocar la torre.
     */
    static std::string getPutTowerRequest(int x, int y);

    /*
     * Retorna la notificación enviada por el server para poner una torre en el escenario.
     * Es un json string.
     */
    static std::string getPutTowerNotification(Json::Value &root);

    /*
     * Retorna la operación asociada al request que se hace con el Json.
     */
    static int getOperation(Json::Value &root);
};


#endif //TP4_TOWERDEFENSE_MESSAGEFACTORY_H
