#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H

static const int MAX_SERVER_NOTIFICATIONS_PER_FRAME = 1;
static const int CANT_TOWERS_BUTTONS = 4;
static const int CANT_TOWERS_BUTTONS_STATES = 4;
static const int TOWER_BUTTONS_X_POS = 1;
static const int TOWER_BUTTONS_Y_POS = 1;
static const int TOWER_BUTTONS_WIDTH = 360;
static const int TOWBER_BUTTONS_HEIGHT = 40;

static const int TIME_FOR_ENABLE_ACTION = 20000;

enum GameStatus {
    GAME_STATUS_UNDECIDED = 0,
    GAME_STATUS_WON = 1,
    GAME_STATUS_LOOSE = 2
};

#include "../common/Socket.h"
#include <string>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_system.h>
#include <map>
#include "../sdl/Constants.h"
#include "../sdl/Texture.h"
#include "../sdl/Tile.h"
#include "../common/Point.h"
#include "../common/SharedBuffer.h"
#include "../common/Thread.h"
#include "../sdl/enemies/Enemy.h"
#include "../sdl/enemies/Abmonible.h"
#include "../sdl/towers/Tower.h"
#include "../sdl/enemies/DrawableHorde.h"
#include "../common/Message.h"
#include "../common/modelo/Mapa.h"
#include <vector>

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

    void handleServerNotifications(SDL_Rect rect);

    void loadPortalSprites();

    void renderText(SDL_Rect &camera, std::string text, int x = 50, int y = 50);

    void initializeGameActors();

    void handleLeftButtonClick(Point &point);

    bool isFirmTerrain(Point &point);

    bool isATowerPoint(Point &point);

    bool isGroundTerrain(Point &point);

    bool isClickOnTowerButton(int mousePosX, int mousePosY) const;

    void handleRightButtonClick(Point point);

    bool hasElement(const std::string &element) const;

    void sendToServer(const std::string &request) const;

    void setToMarkedTile(Point &point);

    void setToFirmTile(Point &point);

    void handleServerPlayerNotifications(SDL_Rect camera);

    void doCastSpellRequest(const Point &point) const;

    void doPutTowerRequest(const Point &point) const;

    void doUpgradeRequest() const;

    void doTowerInfoRequest() const;

    void renderTimeMessages(SDL_Rect &camera);

    void loadTowerInfo(Message message);

    void setToTowerTile(Point point, Tower *tower);

    bool isAValidPutTowerRequest(Point &point);

    /* *
     * Attributes
     * */
    int gameStatus;

    //The window we'll be rendering to
    SDL_Window *gWindow = nullptr;

    //The window renderer
    SDL_Renderer *gRenderer = nullptr;

    SDL_Rect camera;

    //The level tiles
    Tile *tileSet[TOTAL_TILES];

    Texture dotTexture;

    Texture gPromptTextTexture;

    SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

    SDL_Rect gSpriteClipsPortalBlue[30];
    Texture bluePortalTexture;

    SDL_Rect gSpriteClipsPortalRed[30];
    Texture redPortalTexture;

    Texture gSpriteSheetTextureTower;

    SDL_Rect towerButtonsClips[4][4];
    Texture towerButtonsTexture;
    int towerButtonType;
    int towerButtonState;

    //Scene textures
    Texture gTileTextures[TOTAL_TILE_SPRITES];

    // Enemies textures
    Texture *abmonibleTexture = nullptr;
    Texture *blookHawkTexture = nullptr;
    Texture *goatmanTexture = nullptr;
    Texture *greenDaemonTexture = nullptr;
    Texture *spectreTexture = nullptr;
    Texture *zombieTexture = nullptr;

    // Comunication with the game server
    Socket *server = nullptr;
    SharedBuffer *nonPlayerNotifications = nullptr;
    SharedBuffer *playerNotifications = nullptr;

    int clientId;

    std::map<int, DrawableHorde *> hordes;
    std::vector<Tower *> towers;
    std::vector<std::string> playerElements;
    std::string matchName;
    model::Mapa map;

    TTF_Font *font;
    int typeOfTowerToPut;
    int typeOfUpgradeToDo;
    int towerSelected;
    int towerIdThatRequiresInfo;
    bool isCastingSpells;
    int timeOfLastSpell;
    int timeOfLastTowerPutted;
    std::string towerDamageDataMessage;
    std::string towerRangeDataMessage;
    std::string towerReachDataMessage;
    std::string towerSlowdownDataMessage;
};

#endif //TP4_TOWERDEFENSE_GAME_H
