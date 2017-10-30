#ifndef TP4_TOWERDEFENSE_MESSAGEFACTORY_H
#define TP4_TOWERDEFENSE_MESSAGEFACTORY_H

static const char *const OPERATION_KEY = "operation";

#include <string>
#include <jsoncpp/json/json.h>

class MessageFactory {
public:
    static std::string getClientIdNotification(int clientId);
    static std::string getClientIdRequest(std::string message);

    static std::string getGamesRequest(int clientId);
    static std::string getGamesNotification(int clientId,std::string games);

    static std::string getCreateMatchNotification(int gameId,int clientIdWhoCreatedGame);



        /*
         * Retorna la operación asociada al request que se hace con el Json.
         */
    static int getOperation(Json::Value &root);

    /*
     * Retorna el request de texto para agregar una torre, listo para
     * ser enviado al server.
     * clientId: id del cliente que se conecta.
     * x: coordenada x donde se quiere colocar la torre.
     * y: coordenada y donde se quiere colocar la torre.
     * isPut: es true si se está colocando la torre, false en caso contrario.
     */
    static std::string getPutTowerRequest(int clientId, int x, int y, bool isPut);

    /*
     * Retorna la notificación enviada por el server para poner una torre en el escenario.
     * Es un json string.
     */
    static std::string getPutTowerNotification(Json::Value &root);

    /*
     * Retorna el request de texto para marcar un lugar donde poner una torre, listo para
     * ser enviado al server.
     * clientId: id del cliente que se conecta.
     * x: coordenada x donde se quiere colocar la torre.
     * y: coordenada y donde se quiere colocar la torre.
     * isMark: es true si se está marcando, false para 'desmarcar'.
     */
    std::string getMarkTileRequest(int clientId, int x, int y, bool isMark);

    /*
     * Retorna la operación asociada al request que se hace con el Json.
     */
    static std::string getMarkTileNotification(Json::Value &root);

    std::string getCreateMatchRequest(int clientId, int mapId);

    std::string getCreateMatchNotification(Json::Value &root);

    std::string getMapsRequest(int clientId);

    std::string getMapsNotification(Json::Value &root);

    static std::string getCreateFullMatchNotification(int id, int clientId);

    static std::string getAddPlayerToMatchNotification(int gameId, int clientIdWasAdded);

    static std::string getAddPlayerAndRunMatchNotification(int id, int clientId);
};


#endif //TP4_TOWERDEFENSE_MESSAGEFACTORY_H
