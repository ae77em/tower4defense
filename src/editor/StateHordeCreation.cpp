#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

using namespace Editor;

StateHordeCreation::StateHordeCreation(int path_index)
        : index(path_index), enemy(), horde_size(-1), delay_seconds(-1) {}

void StateHordeCreation::handle(const SDL_Event &e, Editor &context) {
    /* Ask enemy type */
    if (enemy == "") {
        context.unsafe_transition(new DataEntry(this,
                [&](const std::string& data) { enemy = data; },
                "1:abominable  2:hawk  3:goatman  "
                "4:daemon  5:spectre  6:zombie "
        ));
    /* Ask number of enemies */
    } else if (horde_size <= 0) {
        //TODO: this would be cleaner inside the lambda
        if (enemy == "1") enemy = "abominable";
        else if (enemy == "2") enemy = "bloodhawk";
        else if (enemy == "3") enemy = "goatman";
        else if (enemy == "4") enemy = "greendaemon";
        else if (enemy == "5") enemy = "spectre";
        else if (enemy == "6") enemy = "zombie";
        else {
            enemy = "";
            return;
        }
        //FIXME: does not validate input
        context.unsafe_transition(new DataEntry(this,
                [&](const std::string& data) {
                    horde_size = stol(data);
                },
                "How many enemies should this horde have? "
        ));
    /* Ask delay in seconds */
    } else if (delay_seconds < 0) {
        //FIXME: does not validate input
        context.unsafe_transition(new DataEntry(this,
                [&](const std::string& data) {
                    delay_seconds = stol(data);
                },
                "Seconds to wait after this horde: "
        ));
    /* Add horde instance to map */
    } else {
        context.getMap().addHorde(enemy, horde_size, index, delay_seconds);
        context.transition(new StateHordeManagement());
    }
}
