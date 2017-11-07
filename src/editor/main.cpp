#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../sdl/Screen.h"
#include <stdexcept>
#include <vector>
#include "../common/Point.h"
#include <fstream>

enum input_mode { TILE, PATH };

int main(int argc, char *argv[]) {
// SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");

    if (TTF_Init() == -1)
        throw std::runtime_error("TTF init failed");

    Mapa mapa(10, 10);
    mapa.setCasilla('~', 1, 1);
    mapa.setCasilla('*', 1, 8);
    mapa.setCasilla('!', 8, 1);
    mapa.setCasilla('@', 8, 8);
    mapa.setCasilla('E', 4, 4);
    mapa.setCasilla('S', 5, 5);

    std::fstream map_file;
    map_file.open("resources/maps/mapa.json", std::ios::in | std::ios::binary);
    if (map_file) {
        // load json into string
        std::string contents;
        map_file.seekg(0, std::ios::end);
        contents.resize(map_file.tellg());
        map_file.seekg(0, std::ios::beg);
        map_file.read(&contents[0], contents.size());
        map_file.close();

        mapa = Mapa(contents);
    }

    Screen screen;
    screen.setDialog("-- TILE --");

    enum input_mode mode = TILE;
    char tile_type_to_put = '~';

    std::vector<Point> path;

    Keybinding keys(default_keybinding);

    bool quit = false;
    while (!quit) {
        screen.clear();

        SDL_Event e;
        while(SDL_PollEvent( &e ) != 0) {
            if (e.type == SDL_QUIT) quit = true;

            if (mode == TILE) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    Point p = screen.mouseCurrentTile();
                    Point d = mapa.dimensiones();
                    if ((p.isPositive()) && (p.x < d.x) && (p.y < d.y))
                        mapa.setCasilla(tile_type_to_put, p.x, p.y);
                }

                // Change tool type
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.water)
                    tile_type_to_put = '~';
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.earth)
                    tile_type_to_put = '*';
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.fire)
                    tile_type_to_put = '!';
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.air)
                    tile_type_to_put = '@';

                // Change mode
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.road) {
                    mode = PATH;
                    screen.setDialog("-- PATH --");
                }
            } else if (mode == PATH) {
                // Simple, unshifted click adds points to path
                if (e.type == SDL_MOUSEBUTTONDOWN
                        && !( KMOD_SHIFT & SDL_GetModState() )) {
                    Point p = screen.mouseCurrentTile();
                    Point d = mapa.dimensiones();
                    if ((p.isPositive()) && (p.x < d.x) && (p.y < d.y))
                        path.push_back(p);
                }

                // Shift-click adds point to path, ends it, and adds it to map
                if (e.type == SDL_MOUSEBUTTONDOWN
                        && ( KMOD_SHIFT & SDL_GetModState() )) {
                    Point p = screen.mouseCurrentTile();
                    Point d = mapa.dimensiones();

                    /* Do something iff the new point is acceptable as
                       an extension of the path, and the result would be
                       a sensible path */
                    if ((p.isPositive()) && (p.x < d.x) && (p.y < d.y)
                            && (path.size() > 0)) {
                        path.push_back(p);
                        mapa.agregarCamino(path);
                        path.clear();
                    }
                }

                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.cancel) {
                    path.clear();
                }

                // Change mode
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.road) {
                    path.clear();
                    mode = TILE;
                    screen.setDialog("-- TILE --");
                }
            }

            screen.handleEvent(e);
        }

        screen.put(mapa);
        for (const auto& camino : mapa.getCaminos())
            screen.trace(camino);
        screen.trace(path);
        screen.draw();
    }

    map_file.open("resources/maps/mapa.json", std::ios::out | std::ios::binary);
    map_file << mapa.serialize();
    map_file.close();

    TTF_Quit();
    SDL_Quit();
}
