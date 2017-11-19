#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

void StateHordeManagement::onTransition(Editor &context) {
    context.getScreen().setDialog("Click on portal to add new horde");
}

void StateHordeManagement::handle(const SDL_Event &e, Editor &context) {
    auto& screen = context.getScreen();
    auto& map = context.getMap();

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        Point p = context.getScreen().mouseCurrentTile();
        if (map.estaDentro(p) && (map.casilla(p.x, p.y) == 'E'))
            screen.setDialog("Clicked on entry portal");
    }

    const auto& keys = context.getKeys();
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.cancel) {
        screen.setDialog("");
        context.transition(new StateTile());
    }
}
