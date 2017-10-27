#include "Screen.h"
#include <stdexcept>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");
    
    Screen screen;

    for (int i = 0; i < 10; ++i) for (int j = 0; j < 10; j++)
        screen.putTile(i, j);

    screen.draw();

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while(SDL_PollEvent( &e ) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                screen.putTower(5, 5);
                screen.draw();
            }
        }
    }

    SDL_Quit();
}
