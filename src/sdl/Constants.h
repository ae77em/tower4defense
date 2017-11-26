#ifndef TP4_TOWERDEFENSE_CONSTANTS_H
#define TP4_TOWERDEFENSE_CONSTANTS_H

#include <vector>
#include <string>

// Screen dimension
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Tile sprite paths
const std::vector<std::string> TILES_IMAGES_PATHS({
      "images/sprites/tile-grass.png",
      "images/sprites/tile-desert.png",
      "images/sprites/tile-ice.png",
      "images/sprites/tile-lava.png",
      "images/sprites/tile-way.png",
      "images/sprites/tile-firm.png",
      "images/sprites/tile-firm-marked.png",
      "images/sprites/tile-firm.png"
});

const int TOTAL_TILE_SPRITES = 8;

const int NUMBER_OF_ENEMY_WALK_SPRITES = 12;
const int NUMBER_OF_ENEMY_WALK_DIRECTIONS = 4;
const int NUMBER_OF_ENEMY_DEATH_SPRITES = 18;
const int NUMBER_OF_ENEMY_DEATH_DIRECTIONS = 4;

// Default measures for enemy sprites
const int WALK_SPRITE_WIDTH_DEFAULT = 105;
const int WALK_SPRITE_HEIGHT_DEFAULT = 119;
const int DEATH_SPRITE_WIDTH_DEFAULT = 200;
const int DEATH_SPRITE_HEIGHT_DEFAULT = 155;

// Sprite measures for Abmonible
const int WALK_SPRITE_WIDTH_ABMONIBLE = 105;
const int WALK_SPRITE_HEIGHT_ABMONIBLE = 119;

const int DEATH_SPRITE_WIDTH_ABMONIBLE = 200;
const int DEATH_SPRITE_HEIGHT_ABMONIBLE = 155;


/* There are eight directions to face: front, front left, left,
   and so on, on a clockwise fashion. We are only interested
   in facing diagonally from the player's perspective, which is why
   we only consider these four directions. */
const int FRONT_LEFT_SPRITE_ROW = 1;
const int BACK_LEFT_SPRITE_ROW = 3;
const int BACK_RIGHT_SPRITE_ROW = 5;
const int FRONT_RIGHT_SPRITE_ROW = 7;

const int SPRITE_DIRECTIONS[4] = {
        FRONT_LEFT_SPRITE_ROW,
        BACK_LEFT_SPRITE_ROW,
        BACK_RIGHT_SPRITE_ROW,
        FRONT_RIGHT_SPRITE_ROW
};


enum TileType {
    TILE_EMPTY = -1,
    TILE_GRASS = 0,
    TILE_DESERT = 1,
    TILE_ICE = 2,
    TILE_LAVA = 3,
    TILE_WAY = 4,
    TILE_FIRM = 5,
    TILE_FIRM_MARKED = 6,
    TILE_TOWER = 7,
    TILE_ENTER_PORTAL = 8,
    TILE_EXIT_PORTAL = 9
};

enum UpgradeTypes {
    UPGRADE_DAMAGE = 0,
    UPGRADE_RANGE = 1,
    UPGRADE_SLOWDOWN = 2,
    UPGRADE_REACH = 3
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

enum TowerButtonsTypes {
    BUTTON_TOWER_AIR = 0,
    BUTTON_TOWER_FIRE = 1,
    BUTTON_TOWER_WATER = 2,
    BUTTON_TOWER_EARTH = 3
};

enum TowerButtonsStates {
    BUTTON_TOWER_ENABLED = 0,
    BUTTON_TOWER_DISABLED = 1,
    BUTTON_TOWER_MOUSEOVER = 2,
    BUTTON_TOWER_SELECTED = 3
};

#endif //TP4_TOWERDEFENSE_CONSTANTS_H
