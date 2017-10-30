#include "Screen.h"
#include <stdexcept>
#include "../sdl/Constants.h"
#include "../sdl/Utils.h"
#include "../common/modelo/Mapa.h"

Screen::Screen() {
    window = SDL_CreateWindow("Tower4Defense", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) throw std::runtime_error("Could not create window"
            + std::string(SDL_GetError()));

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) throw std::runtime_error("Could not create renderer"
            + std::string(SDL_GetError()));

    //Initialize renderer color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (! IMG_Init(imgFlags) || ! imgFlags )
        throw std::runtime_error("Could not initialize SDL_image. Error: "
                + std::string(IMG_GetError()));

    waterTower.loadFromFile("images/water_tower.png", renderer);
    earthTower.loadFromFile("images/earth_tower.png", renderer);
    fireTower.loadFromFile("images/fire_tower.png", renderer);
    airTower.loadFromFile("images/air_tower.png", renderer);
    tile.loadFromFile("images/sprites/tile-grass.png", renderer);
}

Screen::~Screen() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void Screen::draw() {
    dot.move();
    dot.setCamera(camera);
    SDL_RenderPresent(renderer);
}

void Screen::putWaterTower(unsigned x, unsigned y) {
    Point pos = Utils::mapToScreen(x, y);
    pos.x -= camera.x;
    pos.y -= camera.y;

    /* Correccion de la posicion de la base
     *
     *   .    t   ^       ^
     *  . .  ttt  |tile   |tower
     * .   . t t  |       |
     *  . .  t t  |       |
     *   .   t t  v       |
     *       t t  ^d      |
     *       ttt  |       v
     */
    pos.y -= waterTower.getHeight() - tile.getHeight();

    waterTower.render(renderer, pos.x, pos.y);
}

void Screen::putEarthTower(unsigned x, unsigned y) {
    Point pos = Utils::mapToScreen(x, y);
    pos.x -= camera.x;
    pos.y -= camera.y;

    /* Correccion de la posicion de la base */
    pos.y -= earthTower.getHeight() - tile.getHeight();

    earthTower.render(renderer, pos.x, pos.y);
}

void Screen::putFireTower(unsigned x, unsigned y) {
    Point pos = Utils::mapToScreen(x, y);
    pos.x -= camera.x;
    pos.y -= camera.y;

    /* Correccion de la posicion de la base */
    pos.y -= fireTower.getHeight() - tile.getHeight();

    fireTower.render(renderer, pos.x, pos.y);
}

void Screen::putAirTower(unsigned x, unsigned y) {
    Point pos = Utils::mapToScreen(x, y);
    pos.x -= camera.x;
    pos.y -= camera.y;

    /* Correccion de la posicion de la base */
    pos.y -= airTower.getHeight() - tile.getHeight();

    airTower.render(renderer, pos.x, pos.y);
}

void Screen::putTile(unsigned x, unsigned y) {
    Point pos = Utils::mapToScreen(x, y);
    pos.x -= camera.x;
    pos.y -= camera.y;
    tile.render(renderer, pos.x, pos.y);
}

void Screen::handleEvent(SDL_Event &e) {
    std::string s;
    dot.handleEvent(e, s);
}

void Screen::clear() {
    SDL_RenderClear(renderer);
}

void Screen::put(Mapa &map) {
    Point dimensions = map.dimensiones();
    for (int x = 0; x < dimensions.x; ++x)
        for (int y = 0; y < dimensions.y; ++y)
            if (map.casilla(x, y) != '#') putTile(x, y);

    for (int x = 0; x < dimensions.x; ++x)
        for (int y = 0; y < dimensions.y; ++y)
            switch (map.casilla(x, y)) {
                case '~': putWaterTower(x, y);
                          break;
                case '*': putEarthTower(x, y);
                          break;
                case '!': putFireTower(x, y);
                          break;
                case '@': putAirTower(x, y);
            }
}

Point Screen::mouseCurrentTile() {
    return Utils::getMouseRelativePoint(camera);
}
