#include "Screen.h"
#include <stdexcept>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");
    
    Mapa mapa(10, 10);
    Screen screen;

    bool quit = false;
    while (!quit) {
        screen.clear();

        SDL_Event e;
        while(SDL_PollEvent( &e ) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                Point p = screen.mouseCurrentTile();
                Point d = mapa.dimensiones();
                if ((p.isPositive()) && (p.x < d.x) && (p.y < d.y))
                    mapa.setCasilla('*', p.x, p.y);
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                screen.handleEvent(e);
            }
        }

        screen.put(mapa);
        screen.draw();
    }

    SDL_Quit();
}
