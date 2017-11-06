#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../sdl/Screen.h"
#include <stdexcept>
#include <vector>
#include "../common/Point.h"

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
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    Point p = screen.mouseCurrentTile();
                    Point d = mapa.dimensiones();
                    if ((p.isPositive()) && (p.x < d.x) && (p.y < d.y))
                        path.push_back(p);
                }

                // Change mode
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.road) {
                    mode = TILE;
                    screen.setDialog("-- TILE --");
                }
            }

            screen.handleEvent(e);
        }

        screen.put(mapa);
        screen.trace(path);
        screen.draw();
    }

    TTF_Quit();
    SDL_Quit();
}
