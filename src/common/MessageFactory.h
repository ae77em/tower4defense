#ifndef TP4_TOWERDEFENSE_MESSAGEFACTORY_H
#define TP4_TOWERDEFENSE_MESSAGEFACTORY_H

static const char *const OPERATION_KEY = "operation";

static const char *const CLIENT_ID_KEY = "clientId";

#include <string>
#include <jsoncpp/json/json.h>
#include <set>
#include <list>
#include "Message.h"
#include "Point.h"
#include "../server/GameActor.h"
#include "../server/game-actors/enemies/ActorEnemy.h"

class MessageFactory {
public:
    static std::string getClientIdNotification(int clientId);
    static std::string getClientIdRequest(std::string message);

    static std::string getGamesRequest(int clientId);
    static std::string getGamesNotification(int clientId,std::string games);

    static std::string getCreateMatchNotification(int gameId,int clientIdWhoCreatedGame);

    static std::string getNewMatchNotification(std::string matchName);
    static std::string getNewMatchErrorNotification(std::string matchName,std::string cause);



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

    /*
     * Retorna el pedido para obtener todos los mapas existentes para jugar.
     */
    static std::string getExistingMapsRequest(int clientId);

    /*
     * Retorna la notificación con todos los mapas existentes para jugar.
     */
    static std::string getExistingMapsNotification();

    /*
     * Retorna el pedido para obtener todas las partidas existentes para jugar.
     */
    static std::string getExistingMatchesRequest(int clientId);

    /*
     * Retorna la notificación con todas las partidas existentes para jugar.
     */
    static std::string getExistingMatchesNotification(std::vector<std::string> &matches);

    /*
     * Retorna la notificación informando de una nueva partida fue creada.
     */
    static std::string getNewMatchNotification(Message &request, std::set<std::string> &matches);

    /*
     * Retorna el pedido para crear una nueva partida.
     */
    static std::string getNewMatchRequest(int clientId, std::string &mapName, std::string &matchName);

    static std::string getMatchNotAvailableNotification(std::string matchName, std::string errorMessage);

    static std::string getAddPlayerToMatchNotification(std::string gameId,
                                                       int clientIdWasAdded,
                                                       std::vector<std::string> elements);

    static std::string getAddPlayerAndRunMatchNotification(std::string gameID, int clientId);

    static int getClientId(Message &message);

    static int getMatchId(Message &message);

    static std::vector<std::string> getMaps(Message &message);

    static std::string getMapName(Message &message);

    static std::string getMatchName(Message &message);

    static std::string getClientEndConectionNotification(int clientId);

    static std::string getMovementNotification(int enemyId, int moveable, int x, int y, int direction);

    static Point getPoint(Message message);

    static int getDirection(Message message);

    static std::string getStartMatchNotification(std::string name);

    static std::string getMatchStartedNotification(std::string name);

    static std::string getUnavailableElementsNotification(std::list<std::string> elements);

    static std::list<std::string> getElements(Message message);

    static int getEnemyId(Message message);

    static std::string getUnavailableElementsRequest(int clientId, std::string matchName);

    static std::string getEnterMatchRequest(int clientId, std::string matchName, std::vector<std::string> elements);

    std::string getNewMatchRequest(int clientId, std::string matchName);

    static std::string getStartMatchRequest(int clientId, std::string &matchName);

    static std::string getStatusMatchNotification(std::vector<ActorEnemy *> actors);

    static std::vector<std::string> getMatches(Message &message);

    static std::string getEnteredInMatchNotification(int clientId, std::string matchName);

    static std::vector<Message> getMovementNotifications(Message message);
};


#endif //TP4_TOWERDEFENSE_MESSAGEFACTORY_H
