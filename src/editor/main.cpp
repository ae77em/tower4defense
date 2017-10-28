#include "Screen.h"
#include <stdexcept>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");
    
    Screen screen;

    bool quit = false;
    while (!quit) {
        screen.clear();

        SDL_Event e;
        while(SDL_PollEvent( &e ) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                screen.putTower(5, 5);
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                screen.handleEvent(e);
            }
        }

        for (int i = 0; i < 10; ++i) for (int j = 0; j < 10; j++)
            screen.putTile(i, j);

        screen.draw();
    }

    SDL_Quit();
}
