#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H

static const int MAX_SERVER_NOTIFICATIONS_PER_FRAME = 1;
static const int CANT_TOWERS_BUTTONS = 4;
static const int CANT_TOWERS_BUTTONS_STATES = 4;
static const int TOWER_BUTTONS_X_POS = 1;
static const int TOWER_BUTTONS_Y_POS = 1;
static const int TOWER_BUTTONS_WIDTH = 400;
static const int TOWBER_BUTTONS_HEIGHT = 80;

#include "../common/Socket.h"
#include <string>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_system.h>
#include <map>
#include "../sdl/Constants.h"
#include "../sdl/LTexture.h"
#include "../sdl/Tile.h"
#include "../common/Point.h"
#include "../common/SharedBuffer.h"
#include "../common/Thread.h"
#include "../sdl/enemies/Enemy.h"
#include "../sdl/enemies/Abmonible.h"
#include "../sdl/towers/Tower.h"
#include "../sdl/enemies/Horde.h"

class GamePlayWindow : public Thread {
public:
    GamePlayWindow(Socket *socket,
                   SharedBuffer *in,
                   SharedBuffer *other,
                   int clientId,
                   std::vector<std::string> &playerElements,
                   std::string matchName);

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

    void renderText(SDL_Rect &camera, std::string text);

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

    /* A game is, at all times, in one of three states: won, lost,
       or undecided. There is no point (and some risk) in keeping
       the information redundantly in several variables. */
    bool gameWon;
    bool gameLoose;

    //The window we'll be rendering to
    SDL_Window *gWindow = nullptr;

    //The window renderer
    SDL_Renderer *gRenderer = nullptr;

    //The level tiles
    Tile *tileSet[TOTAL_TILES];

    LTexture dotTexture;

    LTexture gPromptTextTexture;

    SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

    SDL_Rect gSpriteClipsPortalBlue[30];
    LTexture bluePortalTexture;

    SDL_Rect gSpriteClipsPortalRed[30];
    LTexture redPortalTexture;

    LTexture gSpriteSheetTextureTower;

    SDL_Rect towerButtonsClips[4][4];
    LTexture towerButtonsTexture;
    int towerButtonType;
    int towerButtonState;

    //Scene textures
    LTexture gTileTextures[TOTAL_TILE_SPRITES];

    // Enemies textures
    LTexture *abmonibleTexture = nullptr;
    LTexture *blookHawkTexture = nullptr;
    LTexture *goatmanTexture = nullptr;
    LTexture *greenDaemonTexture = nullptr;
    LTexture *spectreTexture = nullptr;
    LTexture *zombieTexture = nullptr;

    // Comunication with the game server
    Socket *server = nullptr;
    SharedBuffer *nonPlayerNotifications = nullptr;
    SharedBuffer *playerNotifications = nullptr;

    int clientId;

    std::map<int, Horde *> hordes;
    std::vector<Tower *> towers;
    std::vector<std::string> playerElements;
    std::string matchName;

    TTF_Font *font;
    int typeOfTowerToPut;
    int towerIdThatRequiresInfo;
    bool isCastingSpells;
    int timeOfLastSpell;

    void handleServerPlayerNotifications(SDL_Rect camera);
};

#endif //TP4_TOWERDEFENSE_GAME_H
