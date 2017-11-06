#ifndef TP4_TOWERDEFENSE_CONSTANTS_H
#define TP4_TOWERDEFENSE_CONSTANTS_H

#include <vector>
#include <string>

//
// Screen dimension constants
//
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//
// Tile constants
//
const int CARTESIAN_TILE_WIDTH = 80;
const int CARTESIAN_TILE_HEIGHT = 80;
const int ISO_TILE_WIDTH = 160;
const int ISO_TILE_HEIGHT = 80;
const int ISO_TILE_WIDTH_HALF = ISO_TILE_WIDTH / 2;
const int ISO_TILE_HEIGHT_HALF = ISO_TILE_HEIGHT / 2;
const int SQUARE_TILE_WIDTH = 80; // width of the tile without iso perspective
const int SQUARE_TILE_HEIGHT = 80; // height of the tile without iso perspective
const int TILES_ROWS = 24;
const int TILES_COLUMNS = 16;
const int TOTAL_TILES = TILES_ROWS * TILES_COLUMNS;
const int TOTAL_TILE_SPRITES = 7;

//
// Constants of enemies sprites
//
const int NUMBER_OF_ENEMY_WALK_SPRITES = 12;
const int NUMBER_OF_ENEMY_WALK_DIRECTIONS = 4;

const int NUMBER_OF_ENEMY_DEATH_SPRITES = 18;
const int NUMBER_OF_ENEMY_DEATH_DIRECTIONS = 4;


//The different tile sprites
enum TileType {
    TILE_GRASS = 0,
    TILE_DESERT = 1,
    TILE_ICE = 2,
    TILE_LAVA = 3,
    TILE_WAY = 4,
    TILE_FIRM = 5,
    TILE_EARTH_TOWER = 6
};

enum LTileSpriteMouseEvent {
    BUTTON_SPRITE_DEFAULT = 0,
    BUTTON_SPRITE_MOUSE_DOWN = 1
};

enum MovementDirection {
    X_POSITIVE = 3,
    Y_POSITIVE = 0,
    X_NEGATIVE = 1,
    Y_NEGATIVE = 2
};

enum EnemyType {
    ABMONIBLE = 0,
    BLOOD_HAWK = 1,
    GOATMAN = 2,
    GREEN_DAEMON = 3,
    SPECTRE = 4,
    ZOMBIE = 5
};

const std::vector<std::string> TILES_IMAGES_PATHS({
    "images/sprites/tile-grass.png",
    "images/sprites/tile-desert.png",
    "images/sprites/tile-ice.png",
    "images/sprites/tile-lava.png",
    "images/sprites/tile-way.png",
    "images/sprites/tile-firm.png",
    "images/sprites/tower-earth-alone.png"
});

#endif //TP4_TOWERDEFENSE_CONSTANTS_H
