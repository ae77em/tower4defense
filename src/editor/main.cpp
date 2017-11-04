#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../sdl/Screen.h"
#include <stdexcept>
#include "../common/Point.h"

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

    Screen screen;
    screen.setDialog("Hola mundo!");

    char tile_type_to_put = '~';

    Keybinding keys(default_keybinding);

    bool quit = false;
    while (!quit) {
        screen.clear();

        SDL_Event e;
        while(SDL_PollEvent( &e ) != 0) {
            if (e.type == SDL_QUIT) quit = true;

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

            screen.handleEvent(e);
        }

        screen.put(mapa);
        screen.draw();
    }

    TTF_Quit();
    SDL_Quit();
}
