#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

StateHordeCreation::StateHordeCreation(int path_index)
        : index(path_index), enemies() {}

void StateHordeCreation::handle(const SDL_Event &e, Editor &context) {
    if (e.type != SDL_KEYDOWN) return;

    if (e.key.keysym.sym == SDLK_1) enemies.push_back("abominable");
    else if (e.key.keysym.sym == SDLK_2) enemies.push_back("bloodhawk");
    else if (e.key.keysym.sym == SDLK_3) enemies.push_back("goatman");
    else if (e.key.keysym.sym == SDLK_4) enemies.push_back("greendaemon");
    else if (e.key.keysym.sym == SDLK_5) enemies.push_back("spectre");
    else if (e.key.keysym.sym == SDLK_6) enemies.push_back("zombie");

    else if (enemies.size() != 0
            && (e.key.keysym.sym == SDLK_RETURN
                || e.key.keysym.sym == SDLK_KP_ENTER)) {
        context.getMap().agregarHorda(index, enemies);
        context.transition(new StateHordeManagement());
    }

    const auto& keys = context.getKeys();
    if (e.key.keysym.sym == keys.cancel) {
        context.transition(new StateHordeManagement());
    }
}

void StateHordeCreation::preRender(Editor &context) {
    context.getScreen().setDialog("1:abominable  2:hawk  3:goatman  "
            "4:daemon  5:spectre  6:zombie  ENTER:finish  monsters: "
            + std::to_string(enemies.size()));
}
