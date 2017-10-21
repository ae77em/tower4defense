//
// Created by esteban on 20/10/17.
//

#ifndef TP4_TOWERDEFENSE_GAME_H
#define TP4_TOWERDEFENSE_GAME_H


#include <Socket.h>
#include <string>
#include <SDL_rect.h>

class Tile;

class Game {
public:
    int run(int argc, char *argv[]);
    void interactWithServer(Socket &client, std::string text);
    //bool touchesWall(SDL_Rect box, Tile *tiles[]);
    bool setTiles(Tile *tiles[]);
    //bool checkCollision(SDL_Rect a, SDL_Rect b);
    void close(Tile *tiles[]);
    bool loadMedia(Tile *tiles[]);
    bool init();
};

#endif //TP4_TOWERDEFENSE_GAME_H
