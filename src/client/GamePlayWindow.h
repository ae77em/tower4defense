#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H

static const int MAX_SERVER_NOTIFICATIONS_PER_FRAME = 1;
static const int TOWER_BUTTONS_X_POS = 1;
static const int TOWER_BUTTONS_Y_POS = 1;
static const int TOWER_BUTTONS_WIDTH = 360;
static const int TOWBER_BUTTONS_HEIGHT = 40;

static const int TIME_FOR_ENABLE_ACTION = 20000;
static const int TIME_FOR_SHOW_TEMPORARY_MESSAGE = 5000;

#include "Socket.h"
#include <string>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_system.h>
#include <map>
#include "Constants.h"
#include "Texture.h"
#include "Tile.h"
#include "Point.h"
#include "SharedBuffer.h"
#include "Thread.h"
#include "Abmonible.h"
#include "Tower.h"
#include "DrawableHorde.h"
#include "Message.h"
#include "model/Map.h"
#include "EnterPortal.h"
#include <vector>
#include <Protocol.h>


struct lessThanByPoint {
    bool operator()(Animable *a, Animable *b) {
        bool toReturn;
        Point A(0, 0), B(0, 0);
        /* si por algún motivo falla... */
        if (a) {
            A = a->getPoint();
        } else {
            return false;
        }
        if (b) {
            B = b->getPoint();
        } else {
            return true;
        }

        if (A.x < B.x) {
            toReturn = true;
        } else if (A.x > B.x) {
            toReturn = false;
        } else {
            if (A.y <= B.y) {
                toReturn = true;
            } else {
                toReturn = false;
            }
        }

        return toReturn;
    }
};

class GamePlayWindow : public Thread {
public:
    GamePlayWindow(Socket *socket,
                   SharedBuffer *in,
                   SharedBuffer *other,
                   int clientId,
                   std::vector<std::string> &playerElements,
                   std::string matchName,
                   std::string map);

    virtual ~GamePlayWindow();

    void run() override;

    bool setTiles();

    void close();

    bool loadMedia();

    bool init();

private:
    void handleMouseEvents(SDL_Rect camera, SDL_Event e);

    void handleServerNotifications();

    void
    renderText(SDL_Rect &camera,
               std::string text,
               int x, int y,
               Uint8 r = 0xFF, Uint8 g = 0xFF, Uint8 b = 0x00);

    void handleLeftButtonClick(Point &point);

    bool isFirmTerrain(Point &point);

    bool isATowerPoint(Point &point);

    bool isGroundTerrain(Point &point);

    bool isClickOnTowerButton(int mousePosX, int mousePosY) const;

    void handleRightButtonClick(Point point);

    bool hasElement(const std::string &element) const;

    void sendToServer(const std::string &request);

    void setToMarkedTile(Point &point);

    void setToFirmTile(Point &point);

    void doCastSpellRequest(const Point &point);

    void doPutTowerRequest(const Point &point);

    void doUpgradeRequest();

    void doTowerInfoRequest();

    void renderTimeMessages();

    void loadTowerInfo(Message message);

    void setToTowerTile(Point point, Tower *tower);

    bool isAValidPutTowerRequest(Point &point);

    void renderLevel();

    void addNewHorde(int hordeId, int enemyType, int amount);

    void putTower(int id, int type, int x, int y);

    void loadAnimables();

    void renderMessages();

    bool pointIsInPaths(Point point, std::vector<std::vector<Point>> &vector);

    /* *
     * Attributes
     * */
    int gameStatus = GAME_STATUS_UNDECIDED;

    //The window we'll be rendering to
    SDL_Window *gWindow = nullptr;

    //The window renderer
    SDL_Renderer *gRenderer = nullptr;

    SDL_Rect camera;

    //The level tiles
    std::vector<Tile> tileSet;

    Texture dotTexture;
    Texture promptTextTexture;
    Texture prohibitedTexture;

    SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

    SDL_Rect spriteClipsPortalBlue[30];
    Texture bluePortalTexture;

    SDL_Rect spriteClipsPortalRed[30];
    Texture redPortalTexture;

    Texture towerButtonsTexture;

    //Scene textures
    Texture gTileTextures[TOTAL_TILE_SPRITES];

    // Enemies textures
    Texture *abmonibleTexture = nullptr;
    Texture *blookHawkTexture = nullptr;
    Texture *goatmanTexture = nullptr;
    Texture *greenDaemonTexture = nullptr;
    Texture *spectreTexture = nullptr;
    Texture *zombieTexture = nullptr;

    // Towers textures
    Texture earthTexture;
    Texture airTexture;
    Texture waterTexture;
    Texture fireTexture;

    // Comunication with the game server
    Socket *server = nullptr;
    SharedBuffer *nonPlayerNotifications = nullptr;
    SharedBuffer *playerNotifications = nullptr;

    int clientId;

    std::map<int, DrawableHorde> hordes;
    std::map<int, Tower *> towers;
    std::vector<Animable *> portals;
    std::vector<std::string> playerElements;
    std::string matchName;
    model::Map map;

    std::priority_queue<Animable *, std::vector<Animable *>, lessThanByPoint>
            animables;

    TTF_Font *font;
    int typeOfTowerToPut;
    int typeOfUpgradeToDo;
    int towerSelected;
    bool isCastingSpells;
    int timeOfLastSpell;
    int timeOfLastTowerPutted;
    int timeOfLastUpgradeMessage;
    std::string towerClassDataMessage;
    std::string towerExperiencePointsDataMessage;
    std::string towerDamageDataMessage;
    std::string towerRangeDataMessage;
    std::string towerReachDataMessage;
    std::string towerSlowdownDataMessage;

    std::string towerUpgradeInfoMessage;

    int TILES_ROWS;
    int TILES_COLUMNS;
    int TOTAL_TILES;

    const std::map<char, int> tileIdTranslator = {
        /*
         * Tabla de codigos:
         *    . espacio transitable
         *    # espacio vacio/impasable
         *    x terreno firme (construccion de torres posible)
         *
         *    E entrada
         *    S salida
         *
         *   d desert
         *   g grass
         *   i ice
         *   l lava
         */
        std::make_pair('.', TILE_WAY),
        std::make_pair('#', TILE_EMPTY),
        std::make_pair('x', TILE_FIRM),
        std::make_pair('d', TILE_DESERT),
        std::make_pair('g', TILE_GRASS),
        std::make_pair('i', TILE_ICE),
        std::make_pair('l', TILE_LAVA)
    };

    void animateAnimables();

    void renderProhibited();

    void renderMessageInCenterOfScreen(std::string message);

    void renderSplashScreen();
};

#endif //TP4_TOWERDEFENSE_GAME_H
