#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

StateHordeCreation::StateHordeCreation(int path_index)
        : index(path_index), enemy(), user_input() {}

void StateHordeCreation::onTransition(Editor &context) {
    context.getScreen().setDialog("1:abominable  2:hawk  3:goatman  "
            "4:daemon  5:spectre  6:zombie");
}

void StateHordeCreation::handle(const SDL_Event &e, Editor &context) {
    /* Ask enemy type */
    if (e.type == SDL_KEYDOWN && enemy == "") {
        if (e.key.keysym.sym == SDLK_1) enemy.assign("abominable");
        else if (e.key.keysym.sym == SDLK_2) enemy.assign("bloodhawk");
        else if (e.key.keysym.sym == SDLK_3) enemy.assign("goatman");
        else if (e.key.keysym.sym == SDLK_4) enemy.assign("greendaemon");
        else if (e.key.keysym.sym == SDLK_5) enemy.assign("spectre");
        else if (e.key.keysym.sym == SDLK_6) enemy.assign("zombie");

        /* If not one of the above: invalid key, don't change the dialog */
        else return;

        context.getScreen().setDialog("How many enemies should "
                "this horde have? 0");

    /* Ask number of enemies */
    //TODO: Would be more elegant to put in a separate state
    } else if (enemy != "") {
        /* Update user input  */
        if (e.type == SDL_TEXTINPUT) {
            //FIXME: does not validate input
            user_input += e.text.text;

        /* Handle special keys */
        } else if (e.type == SDL_KEYDOWN) {
            /* Backspace */
            if (e.key.keysym.sym == SDLK_BACKSPACE && user_input.size() != 0) {
                user_input.pop_back();

            /* Enter */
            } else if ((e.key.keysym.sym == SDLK_RETURN)
                    || (e.key.keysym.sym == SDLK_KP_ENTER)) {
                //FIXME: hardcoded 2 second delay
                context.getMap().addHorde(enemy, stol(user_input),
                        index, 2);
                context.transition(new StateHordeManagement());
                return;
            }
        }

        context.getScreen().setDialog("How many enemies should "
                "this horde have? " + user_input);
    }

    const auto& keys = context.getKeys();
    if (e.key.keysym.sym == keys.cancel) {
        context.transition(new StateHordeManagement());
    }
}
