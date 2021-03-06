#ifndef TP4_TOWERDEFENSE_PROTOCOL_H
#define TP4_TOWERDEFENSE_PROTOCOL_H

#include <cstddef>
#include <string>
#include <map>

/* Códigos de requests para acciones previas al juego */
enum ClientRequestsNonPlaying {
    CLIENT_REQUEST_ACCESS_GAME_MENU = 1,
    CLIENT_REQUEST_ACCESS_CONFIGURATION_MENU = 2,
    CLIENT_REQUEST_NEW_MATCH = 3,
    CLIENT_REQUEST_ENTER_MATCH = 4,
    CLIENT_REQUEST_GET_ALL_MAPS = 5,
    CLIENT_REQUEST_GET_ALL_MATCHES = 6,
    CLIENT_REQUEST_GET_UNAVAILABLE_ELEMENTS = 7,
    CLIENT_REQUEST_START_MATCH = 8,
    CLIENT_REQUEST_LEAVE_MATCH = 9
};

/* Códigos de de requests para acciones del juego propiamente dicho */
enum ClientRequestsPlaying {
    CLIENT_REQUEST_PUT_TOWER = 100,
    CLIENT_REQUEST_MARK_TILE = 101,
    CLIENT_REQUEST_CAST_SPELL = 102,
    CLIENT_REQUEST_TOWER_INFO = 103,
    CLIENT_REQUEST_UPGRADE_TOWER = 104
};

enum ServerNonPlayingNotifications {
    SERVER_NOTIFICATION_LEAVE_MATCH = 201,
    SERVER_NOTIFICATION_NEW_MATCH = 202,
    SERVER_NOTIFICATION_ENTER_EXISTING_MATCH = 203,
    SERVER_NOTIFICATION_GET_ALL_MAPS = 204,
    SERVER_NOTIFICATION_GET_ALL_MATCHES = 205,
    SERVER_NOTIFICATION_MATCH_NOT_AVAILABLE = 206,
    SERVER_NOTIFICATION_CLIENT_ID = 207,
    SERVER_NOTIFICATION_START_MATCH = 208,
    SERVER_NOTIFICATION_STARTED_MATCH = 209,
    SERVER_NOTIFICATION_GET_UNAVAILABLE_ELEMENTS = 210,
    SERVER_NOTIFICATION_SCENARIO_STATUS = 211,
    SERVER_NOTIFICATION_END_CLIENT_CONNECTION = 296,
    SERVER_NOTIFICATION_END_CONNECTION = 297
};

enum ServerPlayingNotifications {
    /* Del 300 para arriba, notificaciones que corresponden a eventos
     * disparados por el usuario. */
    SERVER_NOTIFICATION_PUT_TOWER = 300,
    SERVER_NOTIFICATION_MARK_TILE = 301,
    SERVER_NOTIFICATION_CAST_SPELL = 302,
    SERVER_NOTIFICATION_TOWER_INFO = 303,
    SERVER_NOTIFICATION_APPLY_UPGRADE = 304,
    SERVER_NOTIFICATION_CREATE_HORDE = 305,

    /* Del 399 para abajo, notificaciones que corresponden a otros eventos */
    SERVER_NOTIFICATION_MATCH_ENDED = 399,
    SERVER_NOTIFICATION_MOVE_ENEMY = 397
};

enum EnemyType {
    ENEMY_ABMONIBLE = 0,
    ENEMY_BLOOD_HAWK = 1,
    ENEMY_GOATMAN = 2,
    ENEMY_GREEN_DAEMON = 3,
    ENEMY_SPECTRE = 4,
    ENEMY_ZOMBIE = 5
};

enum TowerType {
    TOWER_AIR = 0,
    TOWER_FIRE = 1,
    TOWER_WATER = 2,
    TOWER_EARTH = 3
};

enum GameStatus {
    GAME_STATUS_UNDECIDED = 0,
    GAME_STATUS_WON = 1,
    GAME_STATUS_LOST = 2,
    GAME_STATUS_DISCONECTED = 3
};

const int CARTESIAN_TILE_WIDTH = 80;
const int CARTESIAN_TILE_HEIGHT = 80;

// Tile dimensions without isometric perspective
const int ISO_TILE_WIDTH = 160;
const int ISO_TILE_HEIGHT = 80;
const int ISO_TILE_WIDTH_HALF = ISO_TILE_WIDTH / 2;
const int ISO_TILE_HEIGHT_HALF = ISO_TILE_HEIGHT / 2;

const std::string STR_WATER = "Agua";
const std::string STR_AIR = "Aire";
const std::string STR_FIRE = "Fuego";
const std::string STR_EARTH = "Tierra";
const std::string STR_NONE = "-none-";

static std::string OPERATION_KEY = "operation";
static std::string CLIENT_ID_KEY = "clientId";
static std::string XCOORD_KEY = "xCoord";
static std::string YCOORD_KEY = "yCoord";
static std::string ENEMY_ID_KEY = "enemyId";
static std::string HORDE_ID_KEY = "hordeId";
static std::string MATCH_NAME_KEY = "matchName";
static std::string TOWER_ID_KEY = "towerId";
static std::string IS_VISIBLE_KEY = "isVisible";
static std::string IS_ALIVE_KEY = "isAlive";
static std::string IS_SHOOTING_KEY = "isShooting";
static std::string MATCH_STATUS_KEY = "matchStatus";
static std::string ENERGY_PERCENTAJE_KEY = "energyPercentaje";

class Protocol {
public:
    Protocol() {}
    virtual ~Protocol() {}

    static std::string notificationToString(int notificationType);
    static std::string requestToString(int requestType);
};

#endif
