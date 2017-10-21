//
// Created by esteban on 20/10/17.
//

#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H


#include <Socket.h>
#include <string>
#include <SDL_rect.h>

class IsometricTile;

class Game {
public:
    int run(int argc, char *argv[]);
    void interactWithServer(Socket &client, std::string text);
    //bool touchesWall(SDL_Rect box, Tile *tiles[]);
    bool setTiles(IsometricTile *tiles[]);
    //bool checkCollision(SDL_Rect a, SDL_Rect b);
    void close(IsometricTile *tiles[]);
    bool loadMedia(IsometricTile *tiles[]);
    bool init();
};

#endif //TP4_TOWERDEFENSE_GAME_H
