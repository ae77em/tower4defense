#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H

static const int MAX_SERVER_NOTIFICATIONS_PER_FRAME = 1;

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
private:
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

    SDL_Rect towerButtonsClips = {0, 0, 320, 80 };
    LTexture towerButtonsTexture;

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
    SharedBuffer *toReceive = nullptr;
    SharedBuffer *toSend = nullptr;

    int clientId;

    std::map<int, Horde*> hordes;
    std::vector<Tower*> towers;

public:
    GamePlayWindow(Socket *socket, SharedBuffer *in, SharedBuffer *out, int clientId);
    ~GamePlayWindow();

    void run();
    void interactWithServer(Socket &client, std::string text);
    bool setTiles();
    void close();
    bool loadMedia();
    bool init();

private:
    void handleMouseEvents(SDL_Rect camera, std::string mov_description, SDL_Event e);
    void loadServerNotifications(std::string notification);

    void handleServerNotifications(SDL_Rect rect);

    enum GameEvents {
        GAME_EVENT_PUT_TOWER = 1,
        GAME_EVENT_QUIT_TOWER = 2,
        GAME_EVENT_KILL_ENEMY = 3
    };

    void loadPortalSprites();

    void matarBichoSiLeHiceClick(const SDL_Rect &camera, Enemy &enemy);

    /* A game is, at all times, in one of three states: won, lost,
       or undecided. There is no point (and some risk) in keeping
       the information redundantly in several variables. */
    bool gameWon;
    bool gameLoose;

    void renderText(SDL_Rect &camera, std::string text);

    TTF_Font *font;

    void initializeGameActors();
};

#endif //TP4_TOWERDEFENSE_GAME_H
