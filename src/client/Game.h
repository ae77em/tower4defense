//
// Created by esteban on 20/10/17.
//

#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H


#include <Socket.h>
#include <string>
#include <SDL_rect.h>
#include <SDL_video.h>
#include <SDL_system.h>
#include "Constants.h"
#include "LTexture.h"
#include "Tile.h"
#include "Point.h"
#include "Timer.h"

class Game {
public:
    Game();
    ~Game();

    int run(int argc, char *argv[]);
    void interactWithServer(Socket &client, std::string text);
    //bool touchesWall(SDL_Rect box, Tile *tiles[]);
    bool setTiles(Tile *tiles[]);
    //bool checkCollision(SDL_Rect a, SDL_Rect b);
    void close(Tile *tiles[]);
    bool loadMedia(Tile *tiles[]);
    bool init();

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

    LTimer stepTimer;
    int startTime;

    void handleMouseEvents(Tile *const *tileSet,
                           const SDL_Rect &camera,
                           std::string &mov_description,
                           SDL_Event &e) const;
};

#endif //TP4_TOWERDEFENSE_GAME_H
