#include "Editor.h"

using namespace Editor;

DataEntry::DataEntry(State* previous_state, void (*callback)(std::string&&),
        const std::string& prompt)
        : previous_state(previous_state), callback(callback),
        prompt(prompt), input(), updated(false) {}

void DataEntry::onTransition(Editor &context) {
    context.getScreen().setDialog(prompt + input);
}

void DataEntry::handle(const SDL_Event &e, Editor &context) {
    /* Handle special keys */
    if (e.type == SDL_KEYDOWN) {

        /* Handle backspace */
        if (e.key.keysym.sym == SDLK_BACKSPACE) {
            if (input.size() == 0) return;
            input.pop_back();
            updated = true;

        /* ENTER: transition to previous state and use callback */
        } else if ((e.key.keysym.sym == SDLK_RETURN)
                || (e.key.keysym.sym == SDLK_KP_ENTER)) {
            callback(std::move(input));
            context.transition(previous_state);
        }

    /* Handle text input */
    } else if (e.type == SDL_TEXTINPUT) {
        input += e.text.text;
        updated = true;
    }
}

void DataEntry::preRender(Editor &context) {
    if (updated) context.getScreen().setDialog(prompt + input);
    updated = false;
}
