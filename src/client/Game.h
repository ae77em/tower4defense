#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H

static const int MAX_SERVER_NOTIFICATIONS_PER_FRAME = 50;

#include "../common/Socket.h"
#include <string>
#include <SDL_rect.h>
#include <SDL_video.h>
#include <SDL_system.h>
#include "Constants.h"
#include "LTexture.h"
#include "Tile.h"
#include "Point.h"
#include "Timer.h"
#include "IntermediateBuffer.h"
#include "../common/Thread.h"

class Game : public Thread {
private:
    //The window we'll be rendering to
    SDL_Window *gWindow = NULL;

    //The window renderer
    SDL_Renderer *gRenderer = NULL;

    LTexture gDotTexture;

    SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

    SDL_Rect gSpriteClipsPortalBlue[30];
    LTexture gSpriteSheetTexturePortalBlue;

    SDL_Rect gSpriteClipsPortalRed[30];
    LTexture gSpriteSheetTexturePortalRed;

    //Scene textures
    LTexture gTileTextures[TOTAL_TILE_SPRITES];

    IntermediateBuffer &dataFromServer;
    IntermediateBuffer &dataToServer;

    int currentEventDispatched;

public:
    Game(IntermediateBuffer &in, IntermediateBuffer &out);
    ~Game();

    void run();
    void interactWithServer(Socket &client, std::string text);
    bool setTiles(Tile *tiles[]);
    void close(Tile *tiles[]);
    bool loadMedia(Tile *tiles[]);
    bool init();

private:
    void handleMouseEvents(Tile *const *tileSet,
                           const SDL_Rect &camera,
                           std::string &mov_description,
                           SDL_Event &e) const;
    void loadServerNotifications(std::string notification);

    void handleServerNotifications(Tile *pTile[], SDL_Rect rect);
};

#endif //TP4_TOWERDEFENSE_GAME_H
