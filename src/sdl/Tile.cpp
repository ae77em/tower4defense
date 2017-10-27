#include <iostream>
#include "Tile.h"
#include "Constants.h"
#include "Utils.h"
#include "../common/Protocol.h"

Tile::Tile(int x, int y) {
    //Get the offsets
    containerBoxAttributes.x = x;
    containerBoxAttributes.y = y;

    //Set the collision box
    containerBoxAttributes.w = ISO_TILE_WIDTH;
    containerBoxAttributes.h = ISO_TILE_HEIGHT;

    //Get the tile type
    type = 0;
}

void Tile::render(SDL_Rect &camera,
                           SDL_Rect *gTileClips,
                           SDL_Renderer *gRenderer,
                           LTexture *gTileTextures) {

    Point screenPoint = Utils::mapToScreen(containerBoxAttributes.x, containerBoxAttributes.y);

    SDL_Rect aux = Utils::getBoxByTileType(type);

    int offset = aux.h - ISO_TILE_HEIGHT;

    int isox = screenPoint.x - camera.x;
    int isoy = screenPoint.y - camera.y - offset;

    gTileTextures[type].render(gRenderer,
                                isox,
                                isoy,
                                &gTileClips[type]);
}


void Tile::renderSprite(SDL_Rect &camera,
                        SDL_Rect *gTileClips,
                        SDL_Renderer *gRenderer,
                        LTexture *gTileTextures) {

    Point screenPoint = Utils::mapToScreen(containerBoxAttributes.x, containerBoxAttributes.y);

        int isox = screenPoint.x - camera.x;
        int isoy = screenPoint.y - camera.y;

    gTileTextures->render_sprite(gRenderer, isox, isoy, gTileClips);

}

void Tile::handleEvent(SDL_Event &e, std::string &desc) {
    //If mouse event happened
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        type = static_cast<int>((type == static_cast<int>(LTileSpriteMouseEvent::BUTTON_SPRITE_MOUSE_DOWN))
                                    ? LTileSpriteMouseEvent::BUTTON_SPRITE_DEFAULT
                                    : LTileSpriteMouseEvent::BUTTON_SPRITE_MOUSE_DOWN);
        desc.append("cambio tile...");
    }
}

void Tile::handleServerNotification(int opCode) {
    //If mouse event happened
    switch (opCode){
        case SERVER_NOTIFICATION_PUT_TOWER: {
            type = type == BUTTON_SPRITE_MOUSE_DOWN
                                ? BUTTON_SPRITE_DEFAULT
                                : BUTTON_SPRITE_MOUSE_DOWN;
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
