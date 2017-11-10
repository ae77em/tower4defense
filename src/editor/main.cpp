#include <SDL2/SDL.h>
#include "Editor.h"

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");

    if (TTF_Init() == -1)
        throw std::runtime_error("TTF init failed");

    Editor::Editor editor(new Editor::StateTile());
    editor.run();

    TTF_Quit();
    SDL_Quit();
}
