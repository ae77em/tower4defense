#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Texture.h"
#include "Animation.h"
#include "Dot.h"
#include "Constants.h"
#include "Map.h"
#include "Point.h"
#include <memory>

class Screen {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Dot dot;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    TTF_Font *font;
    Texture dialog;
    Texture tile_desert, tile_grass, tile_ice, tile_lava, tile_firm;
    std::unique_ptr<Animation> waterTower, earthTower, fireTower, airTower;
    std::unique_ptr<Animation> portal_blue, portal_red;

    void putDialog();

    void put(unsigned x, unsigned y, Texture &texture);
    void put(unsigned x, unsigned y, std::unique_ptr<Animation> &animation);

public:
    Screen();
    ~Screen();

    void clear();
    void put(model::Map &map);
    void trace(const std::vector<Point> &path);
    void setDialog(const std::string &text);
    void draw();

    void handleEvent(SDL_Event &e);
    Point mouseCurrentTile() const;
};

#endif
