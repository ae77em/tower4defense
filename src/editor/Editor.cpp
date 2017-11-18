#include "Editor.h"
#include <SDL2/SDL_image.h>
#include <fstream>

Editor::Editor::Editor(State *state) : state(state), screen(),
        map(10, 10), keys(default_keybinding), scrolling_enabled(true) {}

void Editor::Editor::transition(State *newstate) {
    state.reset(newstate);
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

                /* Handle camera scrolling */
                if (scrolling_enabled) screen.handleEvent(e);
            }
        }

        screen.put(map);

        //TODO move path tracing to screen::put(map)
        for (const auto& camino : map.getCaminos())
            screen.trace(camino);

        /* Let state effect last-minute changes */
        state->preRender(*this);
        screen.draw();
    }
}

Screen& Editor::Editor::getScreen() {
    return screen;
}

model::Mapa& Editor::Editor::getMap() {
    return map;
}

const Keybinding& Editor::Editor::getKeys() {
    return keys;
}

void Editor::Editor::load(std::string filename) {
	std::fstream map_file;
    map_file.open(filename, std::ios::in | std::ios::binary);
    if (!map_file) throw std::runtime_error("Could not open file " + filename);

    // Load file contents into string
    std::string contents;
    map_file.seekg(0, std::ios::end);
    contents.resize(map_file.tellg());
    map_file.seekg(0, std::ios::beg);
    map_file.read(&contents[0], contents.size());

    map_file.close();
    map = model::Mapa(contents);
    map.setNombre(filename);
}

void Editor::Editor::save(std::string filename) {
	std::fstream map_file;
	map_file.open(filename, std::ios::out | std::ios::binary);
    map_file << map.serialize();
    map_file.close();
}

void Editor::Editor::new_map(unsigned side) {
    map = model::Mapa(side, side);
}

void Editor::Editor::inhibitScrolling() {
    scrolling_enabled = false;
}

void Editor::Editor::enableScrolling() {
    scrolling_enabled = true;
}
