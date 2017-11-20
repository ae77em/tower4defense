#include "Editor.h"
#include <SDL2/SDL.h>
#include <stdexcept>

static std::string quit(Editor::Editor &context, const std::string& command) {
    // Signal SDL that the application is done
    SDL_Event e;
    e.type = SDL_QUIT;
    SDL_PushEvent(&e);
    return "q";
}

static std::string open(Editor::Editor &context, const std::string& command) {
    try {
        context.load("resources/maps/" + command);
        return "loaded map: " + command;
    } catch (std::runtime_error) {
        return "could not load map: " + command;
    }
}

static std::string save(Editor::Editor &context, const std::string& command) {
    context.save("resources/maps/" + command);
    return "saved " + command;
}

static std::string new_map(Editor::Editor &context,
        const std::string& command) {
    unsigned size;
    try { size = stoul(command); }
    catch (std::invalid_argument) { return "bad argument to new: " + command; }

    context.new_map(size);
    return "";
}

static std::string change_background(Editor::Editor &context,
        const std::string& command) {
    context.getMap().setEstiloFondo(command[0]);
    return "";
}

static std::string delay(Editor::Editor &context, const std::string& command) {
    int delay;
    try {
        delay = stol(command);
    } catch (std::invalid_argument) {
        return "bad argument to delay: " + command;
    }
    context.getMap().setDelay(delay);
    return "delay set";
}

Editor::StateCommand::StateCommand() {
    dispatch_table["_default_"] = [](Editor& context, const std::string& c){
        return std::string("command not recognized");
    };
    dispatch_table["q"] = quit;
    dispatch_table["quit"] = quit;
    dispatch_table["o"] = open;
    dispatch_table["open"] = open;
    dispatch_table["s"] = save;
    dispatch_table["save"] = save;
    dispatch_table["n"] = new_map;
    dispatch_table["new"] = new_map;
    dispatch_table["bg"] = change_background;
    dispatch_table["background"] = change_background;
    dispatch_table["delay"] = delay;
}

void Editor::StateCommand::handle(const SDL_Event &e, Editor &context) {
    bool textUpdated = false;

    /* Handle Backspace, Ctrl+C, Ctrl+V, and Ret specially */
    if (e.type == SDL_KEYDOWN) {

        // When deleting everything return to tile mode
        if (e.key.keysym.sym == SDLK_BACKSPACE) {
            if ((command.length() == 1) || (SDL_GetModState() & KMOD_ALT)) {
                context.enableScrolling();
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
            // Ignore initial ';'
            command.erase(command.begin());

            // Extract the first word
            size_t sp_index = command.find(' ');
            std::string selector = command.substr(0, sp_index);
            command.erase(0, sp_index + 1);

            command_t c;
            try { c = dispatch_table.at(selector); }
            catch (std::out_of_range) { c = dispatch_table.at("_default_"); }

            context.getScreen().setDialog(c(context, command));

            context.enableScrolling();
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

void Editor::StateCommand::onTransition(Editor &context) {
    context.inhibitScrolling();
}
