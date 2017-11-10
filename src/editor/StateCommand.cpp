#include "Editor.h"
#include <SDL2/SDL.h>

void Editor::StateCommand::handle(const SDL_Event &e, Editor &context) {
    bool textUpdated = false;
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE) {
            /* When deleting everything, or on Alt+Backspace,
               return to tile mode */
            if ((command.length() == 0) || (SDL_GetModState() & KMOD_ALT)) {
                context.transition(new StateTile());
                return;
            }
            command.pop_back();
            textUpdated = true;
        } else if ((e.key.keysym.sym == SDLK_c)
                && (SDL_GetModState() & KMOD_CTRL)) {
            //Handle copy
            SDL_SetClipboardText( command.c_str() );
        } else if ((e.key.keysym.sym == SDLK_v)
                && (SDL_GetModState() & KMOD_CTRL)) {
            //Handle paste
            command = SDL_GetClipboardText();
            textUpdated = true;
        } else if ((e.key.keysym.sym == SDLK_RETURN)
                || (e.key.keysym.sym == SDLK_KP_ENTER)) {
            //Execute command and return to tile mode
            command.erase(command.begin());
            //XXX execute command
            context.transition(new StateTile());
            return;
        }
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
