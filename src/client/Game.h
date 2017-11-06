#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H

static const int MAX_SERVER_NOTIFICATIONS_PER_FRAME = 1;

#include "../common/Socket.h"
#include <string>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_system.h>
#include "../sdl/Constants.h"
#include "../sdl/LTexture.h"
#include "../sdl/Tile.h"
#include "../common/Point.h"
#include "Timer.h"
#include "../common/SharedBuffer.h"
#include "../common/Thread.h"
#include "../sdl/enemies/Enemy.h"
#include "../sdl/enemies/Abmonible.h"
#include "../sdl/towers/Tower.h"

class Game : public Thread {
private:
    //The window we'll be rendering to
    SDL_Window *gWindow = NULL;

    //The window renderer
    SDL_Renderer *gRenderer = NULL;

    //The level tiles
    Tile *tileSet[TOTAL_TILES];

    LTexture gDotTexture;

    LTexture gPromptTextTexture;

    SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

    SDL_Rect gSpriteClipsPortalBlue[30];
    LTexture gSpriteSheetTexturePortalBlue;

    SDL_Rect gSpriteClipsPortalRed[30];
    LTexture gSpriteSheetTexturePortalRed;

    LTexture gSpriteSheetTextureEnemyAbominable;
    LTexture gSpriteSheetTextureTower;

    //Scene textures
    LTexture gTileTextures[TOTAL_TILE_SPRITES];

    SharedBuffer &dataFromServer;
    SharedBuffer &dataToServer;

    int eventDispatched;
    int clientId;

public:
    Game(SharedBuffer &in, SharedBuffer &out, int clientId);
    ~Game();

    void run();
    void interactWithServer(Socket &client, std::string text);
    bool setTiles();
    void close();
    bool loadMedia();
    bool init();

private:
    void handleMouseEvents(SDL_Rect camera, std::string mov_description, SDL_Event e, Enemy &enemy);
    void loadServerNotifications(std::string notification);

    void handleServerNotifications(SDL_Rect rect, Enemy &enemy, Tower &tower);

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
};

#endif //TP4_TOWERDEFENSE_GAME_H
