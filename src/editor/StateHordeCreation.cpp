#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

void StateHordeCreation::handle(const SDL_Event &e, Editor &context) {
    const auto& keys = context.getKeys();
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.cancel) {
        context.transition(new StateHordeManagement());
    }
}

void StateHordeCreation::preRender(Editor &context) {
    //TODO Show enemy buttons
}
