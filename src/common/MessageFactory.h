#ifndef TP4_TOWERDEFENSE_MESSAGEFACTORY_H
#define TP4_TOWERDEFENSE_MESSAGEFACTORY_H

static const char *const OPERATION_KEY = "operation";

#include <string>
#include <jsoncpp/json/json.h>
#include <set>
#include "Message.h"

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
    static int getOperation(Message &request);

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
    static std::string getPutTowerNotification(Message &request);

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
    static std::string getMarkTileNotification(Message &request);

    static std::string getCreateMatchRequest(int clientId, std::string mapName);

    static std::string getCreateMatchNotification(Message &request);

    static std::string getExistingMapsRequest(int clientId);

    static std::string getExistingMapsNotification();

    static std::string getExistingMatchesRequest(int clientId);

    static std::string getExistingMatchesNotification(std::set<std::string> &matches);

    static std::string getNewMatchNotification(Json::Value &root, std::set<std::string> &matches);

    static std::string getNewMatchRequest(std::string &mapName, std::string &matchName);

    static std::string getCreateFullMatchNotification(int gameId, int clientId);

    static std::string getAddPlayerToMatchNotification(int gameId, int clientIdWasAdded);

    static std::string getAddPlayerAndRunMatchNotification(int gameID, int clientId);

    static int getClientId(Message &message);

    static int getMatchId(Message &message);
};


#endif //TP4_TOWERDEFENSE_MESSAGEFACTORY_H
