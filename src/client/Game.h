#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H

static const int MAX_SERVER_NOTIFICATIONS_PER_FRAME = 50;

#include "../common/Socket.h"
#include <string>
#include <SDL_rect.h>
#include <SDL_video.h>
#include <SDL_system.h>
#include "../sdl/Constants.h"
#include "../sdl/LTexture.h"
#include "../sdl/Tile.h"
#include "../common/Point.h"
#include "Timer.h"
#include "SharedBuffer.h"
#include "../common/Thread.h"

class Game : public Thread {
private:
    //The window we'll be rendering to
    SDL_Window *gWindow = NULL;

    //The window renderer
    SDL_Renderer *gRenderer = NULL;

    //The level tiles
    Tile *tileSet[TOTAL_TILES];

    LTexture gDotTexture;

    SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

    SDL_Rect gSpriteClipsPortalBlue[30];
    LTexture gSpriteSheetTexturePortalBlue;

    SDL_Rect gSpriteClipsPortalRed[30];
    LTexture gSpriteSheetTexturePortalRed;

    //Scene textures
    LTexture gTileTextures[TOTAL_TILE_SPRITES];

    SharedBuffer &dataFromServer;
    SharedBuffer &dataToServer;

    int currentEventDispatched;

public:
    Game(SharedBuffer &in, SharedBuffer &out);
    ~Game();

    void run();
    void interactWithServer(Socket &client, std::string text);
    bool setTiles(Tile *tiles[]);
    void close(Tile *tiles[]);
    bool loadMedia(Tile *tiles[]);
    bool init();

private:
    void handleMouseEvents(const SDL_Rect &camera,
                           std::string &mov_description,
                           SDL_Event &e) const;
    void loadServerNotifications(std::string notification);

    void handleServerNotifications(SDL_Rect rect);
};

#endif //TP4_TOWERDEFENSE_GAME_H
