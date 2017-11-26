#include "Tile.h"
#include "Constants.h"
#include "SdlUtils.h"
#include "Protocol.h"
#include <iostream>
#include <string>

Tile::Tile(int x, int y, int t) {
    //Get the offsets
    containerBoxAttributes.x = x;
    containerBoxAttributes.y = y;

    //Set the collision box
    containerBoxAttributes.w = ISO_TILE_WIDTH;
    containerBoxAttributes.h = ISO_TILE_HEIGHT;

    //Get the tile type
    type = t;

    isMarked = false;
    tileMarkedTime = 0;
}

void Tile::render(SDL_Rect &camera,
                           SDL_Rect *gTileClips,
                           SDL_Renderer *renderer,
                           Texture *gTileTextures) {
    Point screenPoint = SdlUtils::mapToScreen(containerBoxAttributes.x,
                                           containerBoxAttributes.y);

    SDL_Rect aux = SdlUtils::getBoxByTileType(type);

    int offset = aux.h - ISO_TILE_HEIGHT;

    int isox = screenPoint.x - camera.x;
    int isoy = screenPoint.y - camera.y - offset;

    gTileTextures[type].render(renderer,
                                isox,
                                isoy,
                                &gTileClips[type]);
}


void Tile::renderSprite(SDL_Rect &camera,
                        SDL_Rect *tileClips,
                        SDL_Renderer *renderer,
                        Texture *tileTextures) {
    Point screenPoint = SdlUtils::mapToScreen(containerBoxAttributes.x,
                                           containerBoxAttributes.y);

        int isox = screenPoint.x - camera.x;
        int isoy = screenPoint.y - camera.y;

    tileTextures->renderSprite(renderer, isox, isoy, tileClips);
}

void Tile::handleEvent(SDL_Event &e, std::string &desc) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        type = ((type == LTileSpriteMouseEvent::BUTTON_SPRITE_MOUSE_DOWN)
            ? LTileSpriteMouseEvent::BUTTON_SPRITE_DEFAULT
            : LTileSpriteMouseEvent::BUTTON_SPRITE_MOUSE_DOWN);
        desc.append("cambio tile...");
    }
}

void Tile::handleServerNotification(int opCode) {
    switch (opCode){
        case SERVER_NOTIFICATION_PUT_TOWER: {
            type = TILE_FIRM_MARKED;
            break;
        }
        default:
            std::cout << "no se reconoció la notificación...";
    }
}

int Tile::getType() {
    return type;
}

SDL_Rect Tile::getBox() {
    return containerBoxAttributes;
}

int Tile::getTileMarkedTime() const {
    return tileMarkedTime;
}

void Tile::setMarkedTime(int tileMarkedTime) {
    Tile::tileMarkedTime = tileMarkedTime;
}

bool Tile::itIsMarked() const {
    return isMarked;
}

void Tile::setIsMarked(bool isMarked) {
    Tile::isMarked = isMarked;
}

void Tile::verifyIfMustContinueMarked(){
    if (isMarked){
        if (SDL_GetTicks() - tileMarkedTime > 5000){
            isMarked = false;
        }
    }
}

void Tile::setType(int tileType) {
    type = tileType;
}

void Tile::setTower(Tower *t) {
    tower = t;
}

Tower *Tile::getTower() {
    return tower;
}

bool Tile::isDrawable() {
    return type != TILE_EMPTY;
}
