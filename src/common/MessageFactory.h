#ifndef TP4_TOWERDEFENSE_MESSAGEFACTORY_H
#define TP4_TOWERDEFENSE_MESSAGEFACTORY_H

#include <string>
#include <jsoncpp/json/json.h>
#include <set>
#include <list>
#include <vector>
#include "Message.h"
#include "Point.h"
#include "../server/GameActor.h"
#include "../server/game-actors/enemies/ActorEnemy.h"
#include "../server/game-actors/enemies/Horde.h"
#include "../server/game-actors/towers/ActorTower.h"

class MessageFactory {
public:
    static std::string getClientIdNotification(int clientId);

    static std::string getClientIdRequest(std::string message);

    static std::string getGamesRequest(int clientId);

    static std::string getGamesNotification(int clientId, std::string games);

    static std::string
    getNewMatchErrorNotification(int clientId, std::string matchName,
                                 std::string cause);

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
    static std::string
    getPutTowerRequest(std::string matchName, int towerType, int x, int y);

    /*
     * Retorna la notificación enviada por el server para poner una torre en el
     * escenario.
     * Es un json string.
     */
    static std::string getPutTowerNotification(Message &request);

    /*
     * Retorna el request de texto para marcar un lugar donde poner una torre,
     * listo para
     * ser enviado al server.
     * clientId: id del cliente que se conecta.
     * x: coordenada x donde se quiere colocar la torre.
     * y: coordenada y donde se quiere colocar la torre.
     * isMark: es true si se está marcando, false para 'desmarcar'.
     */
    static std::string getMarkTileRequest(std::string matchName, int x, int y);

    /*
     * Retorna la operación asociada al request que se hace con el Json.
     */
    static std::string getMarkTileNotification(int x, int y);

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
    static std::string
    getExistingMatchesNotification(std::vector<std::string> &matches);

    /*
     * Retorna la notificación informando de una nueva partida fue creada.
     */
    static std::string
    getNewMatchNotification(int clientId, std::string matchName);


    /*
     * Retorna el pedido para crear una nueva partida.
     */
    static std::string getNewMatchRequest(int clientId, std::string &mapName,
                                          std::string &matchName);

    static std::string getMatchNotAvailableNotification(std::string matchName,
                                                  std::string errorMessage);

    static std::string getAddPlayerToMatchNotification(std::string gameId,
                                              int clientIdWasAdded,
                                              std::list<std::string> elements);

    static std::string
    getAddPlayerAndRunMatchNotification(std::string gameID, int clientId);

    static int getClientId(Message &message);

    static int getMatchId(Message &message);

    static std::vector<std::string> getMaps(Message &message);

    static std::string getMapName(Message &message);

    static std::string getMatchName(Message &message);

    static std::string getClientEndConectionNotification(int clientId);

    static std::string
    getMovementNotification(int enemyId, int moveable, int x, int y,
                            int direction);

    static Point getPoint(Message message);

    static int getDirection(Message message);

    static std::string getStartMatchNotification(std::string name);

    static std::string getMatchStartedNotification(std::string name);

    static std::string
    getUnavailableElementsNotification(std::list<std::string> elements);

    static std::list<std::string> getElements(Message message);

    static int getEnemyId(Message message);

    static std::string
    getUnavailableElementsRequest(int clientId, std::string matchName);

    static std::string getEnterMatchRequest(int clientId, std::string matchName,
                                            std::vector<std::string> elements);

    static std::string
    getStartMatchRequest(int clientId, std::string &matchName);

    static std::vector<std::string> getMatches(Message &message);

    static std::string
    getEnteredInMatchNotification(int clientId, std::string matchName);

    static std::vector<Message> getMovementNotifications(Message message);

    static int getHordeId(Message message);

    static std::string getMatchEndedNotification();

    static std::string
    getTowerInfoRequest(int clientId, std::string matchName, int towerId);

    static std::string getCastSpellRequest(std::string matchName, int x, int y);

    static std::string getPutTowerGameRequest(int towType, int x, int y);

    static std::string getCastSpellGameRequest(int x, int y);

    static std::string
    getUpgradeRequest(std::string matchName, int towerId, int upgradeType);

    static std::string getUpgradeTowerGameRequest(int towerId, int upgradeType);

    static std::string getTowerInfoGameRequest(int towerId);

    static std::string
    getTowerInfoNotification(int towerId,
                             int damage,
                             int range,
                             int reach = 0,
                             int slowDown = 0);

    static std::string getCastSpellNotification(int x, int y);

    static std::string getPutTowerNotification(int towerType, int x, int y);
};

#endif //TP4_TOWERDEFENSE_MESSAGEFACTORY_H
