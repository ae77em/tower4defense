#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

StateHordeCreation::StateHordeCreation(int path_index)
        : index(path_index), enemy() {}

void StateHordeCreation::onTransition(Editor &context) {
    context.getScreen().setDialog("1:abominable  2:hawk  3:goatman  "
            "4:daemon  5:spectre  6:zombie");
}

void StateHordeCreation::handle(const SDL_Event &e, Editor &context) {
    if (e.type != SDL_KEYDOWN) return;

    /* Ask enemy type */
    if (enemy == "") {
        if (e.key.keysym.sym == SDLK_1) enemy.assign("abominable");
        else if (e.key.keysym.sym == SDLK_2) enemy.assign("bloodhawk");
        else if (e.key.keysym.sym == SDLK_3) enemy.assign("goatman");
        else if (e.key.keysym.sym == SDLK_4) enemy.assign("greendaemon");
        else if (e.key.keysym.sym == SDLK_5) enemy.assign("spectre");
        else if (e.key.keysym.sym == SDLK_6) enemy.assign("zombie");

        /* If not one of the above: invalid key, don't change the dialog */
        else return;

        context.getScreen().setDialog("How many? xxx");

    /* Ask number of enemies */
    //TODO: Would be more elegant to put in a separate state
    } else {
        // Missing implementation
    }

    const auto& keys = context.getKeys();
    if (e.key.keysym.sym == keys.cancel) {
        context.transition(new StateHordeManagement());
    }
}
