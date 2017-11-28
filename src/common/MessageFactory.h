#ifndef TP4_TOWERDEFENSE_MESSAGEFACTORY_H
#define TP4_TOWERDEFENSE_MESSAGEFACTORY_H

static const char *const MAP_NAME_KEY = "mapName";

#include <string>
#include <jsoncpp/json/json.h>
#include <set>
#include <list>
#include <vector>
#include "Message.h"
#include "Point.h"

class MessageFactory {
public:
    /*
     * Retorna la notificación client id de cliente nuevo conectado.
     */
    static std::string getClientIdNotification(int clientId);

    /*
     * Retorna notificación de error de creación de nueva partida.
     */
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
     * listo para ser enviado al server.
     * clientId: id del cliente que se conecta.
     * x: coordenada x donde se quiere colocar la torre.
     * y: coordenada y donde se quiere colocar la torre.
     * isMark: es true si se está marcando, false para 'desmarcar'.
     */
    static std::string getMarkTileRequest(std::string matchName, int x, int y);

    static std::string getMarkTileNotification(int x, int y);

    static std::string getExistingMapsRequest(int clientId);

    /*
     * Retorna la notificación con todos los mapas existentes para jugar.
     */
    static std::string getExistingMapsNotification(
            std::vector<std::string> mapsNames);

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

    /*
     * Retorna la notificación indicando que la partida a la cual se intenta
     * unir no está más disponible.
     */
    static std::string getMatchNotAvailableNotification(std::string matchName,
                                                  std::string errorMessage);

    /*
     * Retorna la notificación indicando que el jugador se unió a la partida.
     */
    static std::string getAddPlayerToMatchNotification(std::string gameId,
                                              int clientIdWasAdded,
                                              std::list<std::string> elements);

    /*
     * Retorna la notificación indicando que el jugador se unió a la partida.
     */
    static std::string
    getAddPlayerAndRunMatchNotification(std::string gameID, int clientId);

    /*
     * Retorna el dato client id de un mensaje. Si no lo tiene retorna 0.
     */
    static int getClientId(Message &message);

    /*
     * Retorna el los mapas de un mensaje.
     */
    static std::vector<std::string> getMaps(Message &message);

    /*
     * Retorna el nombre de la partida de un mensaje.
     */
    static std::string getMatchName(Message &message);

    /*
     * Retorna la notificación de que un jugador cerró la conexión con el juego.
     */
    static std::string getClientEndConectionNotification(int clientId);

    /*
     * Retorna un punto del mensaje. Debe tener un sólo punto.
     */
    static Point getPoint(Message message);

    static int getDirection(Message message);

    /*
     * Retorna la notificación de inicio de partida.
     */
    static std::string
    getStartMatchNotification(std::string name, std::string serializedMap);

    /*
     * Retorna la notificación de partida ya iniciada.
     */
    static std::string getMatchStartedNotification(std::string name);

    /*
     * Retorna la notificación con los elementos que ya no están disponibles
     * en una partida dada.
     */
    static std::string
    getUnavailableElementsNotification(std::list<std::string> elements);

    /*
     * Retorna los elementos de juego (tierra, aire, etc) contenidos en un
     * mensaje.
     */
    static std::list<std::string> getElements(Message message);

    /*
     * Retorna el id de enemigo contenido en un mensaje, si lo tiene.
     */
    static int getEnemyId(Message message);

    /*
     * Retorna el pedido para obtener los elementos que ya no están
     * disponibles en una partida dada.
     */
    static std::string
    getUnavailableElementsRequest(int clientId, std::string matchName);

    /*
     * Retorna el request para entrar a una partida.
     */
    static std::string getEnterMatchRequest(int clientId, std::string matchName,
                                            std::vector<std::string> elements);

    /*
     * Retorna el request para iniciar una partida.
     */
    static std::string
    getStartMatchRequest(int clientId, std::string &matchName);

    /*
     * Retorna las partidas que pueda contener un mensaje.
     */
    static std::vector<std::string> getMatches(Message &message);

    /*
     * Retorna las notificaciones de movimiento que pueda contener un mensaje.
     */
    static std::vector<Message> getMovementNotifications(Message message);

    /*
     * Retorna id de hora que pueda contener un mensaje.
     */
    static int getHordeId(Message message);

    /*
     * Retorna el pedido de info de una torre.
     */
    static std::string
    getTowerInfoRequest(int clientId, std::string matchName, int towerId);

    /*
     * Retorna el pedido de lanzamiento de hechizo.
     */
    static std::string getCastSpellRequest(std::string matchName, int x, int y);

    /*
     * Retorna el pedido de upgrade de torre.
     */
    static std::string
    getUpgradeRequest(int clientId, std::string matchName,
                          int towerId, int upgradeType);

    /*
     * Retorna la notificación de lanzamiento de hechizo.
     */
    static std::string getCastSpellNotification(int x, int y);

    /*
     * Retorna la notificación de poner torre.
     */
    static std::string getPutTowerNotification(int towerType, int x, int y);

    /*
     * Retorna el mapa serializado.
     */
    static std::string getSerializedMap(Message message);

    static std::string getLeaveMatchRequest(int clientId, std::string matchName);

    static std::string getLeaveMatchNotification();

    static std::string getEndConectionNotification();
};

#endif //TP4_TOWERDEFENSE_MESSAGEFACTORY_H
