#ifndef TP4_TOWERDEFENSE_CONSTANTS_H
#define TP4_TOWERDEFENSE_CONSTANTS_H

#include <vector>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//The dimensions of the level
//int LEVEL_WIDTH = 2560;
//int LEVEL_HEIGHT = 960;

//Tile constants
const int ISO_TILE_WIDTH = 160;
const int ISO_TILE_HEIGHT = 80;
const int ISO_TILE_WIDTH_HALF = ISO_TILE_WIDTH / 2;
const int ISO_TILE_HEIGHT_HALF = ISO_TILE_HEIGHT / 2;
const int SQUARE_TILE_WIDTH = 80; // width of the tile without iso perspective
const int SQUARE_TILE_HEIGHT = 80; // height of the tile without iso perspective
const int TILES_ROWS = 12;
const int TILES_COLUMNS = 16;
const int TOTAL_TILES = TILES_ROWS * TILES_COLUMNS;
const int TOTAL_TILE_SPRITES = 2;

//The different tile sprites
enum TileType {
    GRASS = 0,
    EARTH_TOWER = 1
};

enum LTileSpriteMouseEvent {
    BUTTON_SPRITE_DEFAULT = 0,
    BUTTON_SPRITE_MOUSE_DOWN = 1
};


const std::vector<std::string> TILES_IMAGES_PATHS({
    "images/sprites/tile-grass.png",
    "images/sprites/tower-earth-alone.png"
});

#endif //TP4_TOWERDEFENSE_CONSTANTS_H
