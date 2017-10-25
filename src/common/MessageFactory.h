#ifndef TP4_TOWERDEFENSE_MESSAGEFACTORY_H
#define TP4_TOWERDEFENSE_MESSAGEFACTORY_H

#include <string>

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
    static std::string getPutTowerNotification();
};


#endif //TP4_TOWERDEFENSE_MESSAGEFACTORY_H
