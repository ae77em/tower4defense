#ifndef TP4_TOWERDEFENSE_DOT_H
#define TP4_TOWERDEFENSE_DOT_H

#include <SDL_rect.h>
#include "../sdl/LTexture.h"
#include "../sdl/Tile.h"

//The dot that will move around on the screen
class Dot {
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    //Maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    //Initializes the variables
    Dot();

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event &e, std::string &desc);

    //Moves the dot and check collision against tiles
    void move();

    //Centers the camera over the dot
    void setCamera(SDL_Rect &camera);

    //Shows the dot on the screen
    void render(LTexture &gDotTexture, SDL_Rect &camera, SDL_Renderer
    *gRenderer);

private:
    //Collision box of the dot
    SDL_Rect mBox;

    //The velocity of the dot
    int mVelX, mVelY;
};

#endif //TP4_TOWERDEFENSE_DOT_H
