#include "Screen.h"
#include "Texture.h"
#include "Animation.h"
#include <stdexcept>
#include <vector>
#include "Constants.h"
#include "Utils.h"
#include "../common/modelo/Mapa.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>

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

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (! IMG_Init(imgFlags) || ! imgFlags )
        throw std::runtime_error("Could not initialize SDL_image. Error: "
                + std::string(IMG_GetError()));

    tile_desert.loadFromFile("images/sprites/tile-desert.png", renderer);
    tile_grass.loadFromFile("images/sprites/tile-grass.png", renderer);
    tile_ice.loadFromFile("images/sprites/tile-ice.png", renderer);
    tile_lava.loadFromFile("images/sprites/tile-lava.png", renderer);
    tile_firm.loadFromFile("images/sprites/tile-firm.png", renderer);

    waterTower.loadFromFile("images/water_tower.png", renderer);
    earthTower.loadFromFile("images/earth_tower.png", renderer);
    fireTower.loadFromFile("images/fire_tower.png", renderer);
    airTower.loadFromFile("images/air_tower.png", renderer);

    portal_blue.reset(new GridAnimation(renderer,
            "images/sprites/portal-blue2.png", 30, 1));
    portal_red.reset(new GridAnimation(renderer,
            "images/sprites/portal-red.png", 10, 3));

    font = TTF_OpenFont("resources/fonts/UbuntuMono-R.ttf", 16);
    if (! font) throw std::runtime_error("Could not load font");
    setDialog("");
}

Screen::~Screen() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void Screen::draw() {
    dot.move();
    dot.setCamera(camera);

    putDialog();

    SDL_RenderPresent(renderer);
}

void Screen::put(unsigned x, unsigned y, Texture &texture) {
    Point pos = Utils::mapToScreen(x, y);

    // Correccion por camara
    pos.x -= camera.x;
    pos.y -= camera.y;

    /* Correccion vertical */
    pos.y -= texture.getHeight() - tile_desert.getHeight();

    /* Correccion horizontal */
    pos.x += (tile_desert.getWidth() - texture.getWidth()) / 2;

    texture.render(renderer, pos.x, pos.y);
}

void Screen::put(unsigned x, unsigned y,
        std::unique_ptr<Animation> &animation) {
    Point pos = Utils::mapToScreen(x, y);

    // Correccion por camara
    pos.x -= camera.x;
    pos.y -= camera.y;

    /* Correccion vertical */
    pos.y -= animation->getHeight() - tile_desert.getHeight();

    /* Correccion horizontal */
    pos.x += (tile_desert.getWidth() - animation->getWidth()) / 2;

    animation->renderFrame(SDL_GetTicks() / 33, pos.x, pos.y);
}

void Screen::putDialog() {
    dialog.render(renderer, 40, camera.h - 40);
}

void Screen::handleEvent(SDL_Event &e) {
    std::string s;
    dot.handleEvent(e);
}

void Screen::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);
}

void Screen::put(model::Mapa &map) {
    Texture &curr_tile_style = (map.getEstiloFondo() == 'd') ? tile_desert
        : (map.getEstiloFondo() == 'g') ? tile_grass
        : (map.getEstiloFondo() == 'i') ? tile_ice
        : (map.getEstiloFondo() == 'l') ? tile_lava
        : tile_desert;
    Point dimensions = map.dimensiones();
    for (int x = 0; x < dimensions.x; ++x)
        for (int y = 0; y < dimensions.y; ++y)
            switch (map.casilla(x, y)) {
                case '#': break;
                case '.': put(x, y, curr_tile_style); break;
                case 'x': put(x, y, tile_firm); break;

                case 'E': case 'S':
                    put(x, y, curr_tile_style); break;

                case '~': case '*': case '!': case '@':
                    put(x, y, tile_firm); break;
            }

    for (int x = 0; x < dimensions.x; ++x)
        for (int y = 0; y < dimensions.y; ++y)
            switch (map.casilla(x, y)) {
                case '~': put(x, y, waterTower); break;
                case '*': put(x, y, earthTower); break;
                case '!': put(x, y, fireTower); break;
                case '@': put(x, y, airTower); break;
                case 'E': put(x, y, portal_blue); break;
                case 'S': put(x, y, portal_red); break;
            }
}

Point Screen::mouseCurrentTile() {
    return Utils::getMouseRelativePoint(camera);
}

void Screen::setDialog(const std::string &text) {
    if (! dialog.generateFromText(text, renderer, font))
        throw std::runtime_error("Failed to generate dialog \""
                + text + "\"");
}

void Screen::trace(const std::vector<Point> &path) {
    if (path.size() < 2) return;

    std::vector<SDL_Point> visual_path;
    visual_path.reserve(path.size());
    for (const auto& point : path) {
        auto screen_point = Utils::mapToScreen(point.x, point.y);
        screen_point.x -= camera.x;
        screen_point.y -= camera.y;
        screen_point.x += tile_desert.getWidth() / 2;
        screen_point.y += tile_desert.getHeight() / 2;

        SDL_Point sdl_point = {screen_point.x, screen_point.y};
        visual_path.push_back(sdl_point);
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (SDL_RenderDrawLines(renderer, visual_path.data(), visual_path.size())
            < 0)
        throw std::runtime_error("Could not draw lines "
                + std::string(SDL_GetError()));
}
