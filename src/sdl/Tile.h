#ifndef TP4_TOWERDEFENSE_ISOMETRICTILE_H
#define TP4_TOWERDEFENSE_ISOMETRICTILE_H

#include <string>
#include <SDL_rect.h>
#include <SDL_system.h>
#include <SDL_events.h>
#include "../sdl/LTexture.h"


//The tile
class Tile {
public:
    //Initializes position and type
    Tile(int x, int y);

    //Shows the tile
    void render(SDL_Rect &camera,
                SDL_Rect *gTileClips,
                SDL_Renderer *gRenderer,
                LTexture *gTileTextures);

    void renderSprite(SDL_Rect &camera,
                      SDL_Rect *gTileClips,
                      SDL_Renderer *gRenderer,
                      LTexture *gTileTextures);

    //Takes mouse events
    void handleEvent(SDL_Event &e, std::string &desc);

    //Get the tile type
    int getType();

    //Set the tile type
    void setType(int tileType);

    //Get the collision box
    SDL_Rect getBox();

private:
    //The attributes of the tile
    SDL_Rect containerBoxAttributes;

    //The tile type
    int type;
};



#endif //TP4_TOWERDEFENSE_ISOMETRICTILE_H
