#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

void StateHordeManagement::onTransition(Editor &context) {
    context.getScreen().setDialog("Click on portal to add new horde. "
            "hordes: " + std::to_string(context.getMap().getHordas().size()));
}

void StateHordeManagement::handle(const SDL_Event &e, Editor &context) {
    auto& screen = context.getScreen();
    auto& map = context.getMap();

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        Point p = context.getScreen().mouseCurrentTile();
        if (map.estaDentro(p) && (map.casilla(p.x, p.y) == 'E')) {
            // Find the path that starts on the clicked tile
            int path_index = -1;
            const auto& paths = map.getCaminos();
            for (unsigned i = 0; i < paths.size(); i++) {
                if (paths[i][0].x == p.x && paths[i][0].y == p.y) {
                    path_index = (int)i;
                    break;
                }
            }

            // Could not find path
            if (path_index == -1) return;

            context.transition(new StateHordeCreation(path_index));
        }
    }

    const auto& keys = context.getKeys();
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.cancel) {
        screen.setDialog("");
        context.transition(new StateTile());
    }
}
