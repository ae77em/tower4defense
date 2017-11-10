#include "Editor.h"
#include <SDL2/SDL.h>

void Editor::StateCommand::handle(const SDL_Event &e, Editor &context) {
    bool textUpdated = false;

    /* Handle Backspace, Ctrl+C, Ctrl+V, and Ret specially */
    if (e.type == SDL_KEYDOWN) {

        // When deleting everything return to tile mode
        if (e.key.keysym.sym == SDLK_BACKSPACE) {
            if ((command.length() == 1) || (SDL_GetModState() & KMOD_ALT)) {
                context.transition(new StateTile());
                return;
            }
            command.pop_back();
            textUpdated = true;

        } else if ((e.key.keysym.sym == SDLK_c)
                && (SDL_GetModState() & KMOD_CTRL)) {
            SDL_SetClipboardText( command.c_str() );

        } else if ((e.key.keysym.sym == SDLK_v)
                && (SDL_GetModState() & KMOD_CTRL)) {
            command = SDL_GetClipboardText();
            textUpdated = true;

        //Execute command and return to tile mode
        } else if ((e.key.keysym.sym == SDLK_RETURN)
                || (e.key.keysym.sym == SDLK_KP_ENTER)) {
            //Execute command and return to tile mode
            command.erase(command.begin());
            //XXX execute command
            context.transition(new StateTile());
            return;
        }

    /* Put text input into the state string and update screen */
    } else if (e.type == SDL_TEXTINPUT) {
        //Not copy or pasting
        if ( !((e.text.text[0] == 'c' || e.text.text[0] == 'C'
                        || e.text.text[0] == 'v' || e.text.text[0] == 'V')
                    && SDL_GetModState() & KMOD_CTRL )) {
            //Append character
            command += e.text.text;
            textUpdated = true;
        }
    }

    if (textUpdated) context.getScreen().setDialog(command);
}
