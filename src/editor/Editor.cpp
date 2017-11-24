#include "Editor.h"
#include <SDL2/SDL_image.h>
#include <fstream>

Editor::Editor::Editor(State *state) : state(state), screen(),
        map(10, 10), keys(default_keybinding), scrolling_enabled(true) {}

void Editor::Editor::transition(State *newstate) {
    state.reset(newstate);
    state->onTransition(*this);
}

void Editor::Editor::unsafe_transition(State *newstate) {
    state.release();
    state = std::unique_ptr<State>(newstate);
    state->onTransition(*this);
}

void Editor::Editor::run() {
    bool quit = false;
    while (!quit) {
        screen.clear();

        /* Poll and handle sdl events */
        SDL_Event e;
        while(SDL_PollEvent( &e ) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                state->handle(e, *this);
                if (scrolling_enabled) screen.handleEvent(e);
            }
        }

        screen.put(map);

        //TODO move path tracing to screen::put(map)
        for (const auto& camino : map.getPaths())
            screen.trace(camino);

        /* Let state effect last-minute changes */
        state->preRender(*this);
        screen.draw();
    }
}

Screen& Editor::Editor::getScreen() {
    return screen;
}

model::Map& Editor::Editor::getMap() {
    return map;
}

const Keybinding& Editor::Editor::getKeys() {
    return keys;
}

void Editor::Editor::load(std::string filename) {
    map = model::Map::loadFromFile(filename);
}

void Editor::Editor::save(std::string filename) {
	std::fstream map_file;
	map_file.open(filename, std::ios::out | std::ios::binary);
    map_file << map.serialize();
    map_file.close();
}

void Editor::Editor::new_map(unsigned side) {
    map = model::Map(side, side);
}

void Editor::Editor::inhibitScrolling() {
    scrolling_enabled = false;
}

void Editor::Editor::enableScrolling() {
    scrolling_enabled = true;
}
