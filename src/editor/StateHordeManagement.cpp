#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

void StateHordeManagement::onTransition(Editor &context) {
    context.getScreen().setDialog("Click on portal to add new horde");
}

void StateHordeManagement::handle(const SDL_Event &e, Editor &context) {
    const auto& keys = context.getKeys();
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.cancel) {
        auto& screen = context.getScreen();
        screen.setDialog("");
        context.transition(new StateTile());
    }
}
