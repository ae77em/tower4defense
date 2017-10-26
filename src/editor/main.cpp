#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>
#include "../sdl/LTexture.h"
#include "../common/Point.h"
#include "../sdl/Utils.h"

int main(int argc, char *argv[]) {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

// SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");

    SDL_Window *gWindow = SDL_CreateWindow("Tower4Defense",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
        SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!gWindow) throw std::runtime_error("Could not create window");

    SDL_Renderer *gRenderer = SDL_CreateRenderer(gWindow, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer) throw std::runtime_error("Could not create renderer");

    //Initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (! IMG_Init(imgFlags) || ! imgFlags )
        throw std::runtime_error("Could not initialize SDL_image. Error: "
                + std::string(IMG_GetError()));

// Draw the map
    LTexture tile;
    tile.loadFromFile("images/sprites/tile-grass.png", gRenderer);

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; j++) {
            Point pos = Utils::mapToScreen(i, j);
            tile.render(gRenderer, pos.x, pos.y);
        }

    SDL_RenderPresent(gRenderer);

    while (true) if (std::cin.get() == 'q') break;
}
