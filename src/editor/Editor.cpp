#include "Editor.h"
#include <SDL2/SDL_image.h>
#include <fstream>

Editor::Editor::Editor(State *state) : state(state), screen(),
        map(10, 10), keys(default_keybinding) {
    map.setCasilla('~', 1, 1);
    map.setCasilla('*', 1, 8);
    map.setCasilla('!', 8, 1);
    map.setCasilla('@', 8, 8);
    map.setCasilla('E', 4, 4);
}

void Editor::Editor::transition(State *newstate) {
    state.reset(newstate);
    state->onTransition(*this);
}

void Editor::Editor::run() {
    bool quit = false;
    while (!quit) {
        screen.clear();

        SDL_Event e;
        while(SDL_PollEvent( &e ) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                state->handle(e, *this);
                screen.handleEvent(e);
            }
        }

        screen.put(map);
        //TODO move path tracing to screen::put(map)
        for (const auto& camino : map.getCaminos())
            screen.trace(camino);

        state->preRender(*this);

        screen.draw();
    }
}

Screen& Editor::Editor::getScreen() {
    return screen;
}

Mapa& Editor::Editor::getMap() {
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
    map = Mapa(contents);
}

void Editor::Editor::save(std::string filename) {
	std::fstream map_file;
	map_file.open(filename, std::ios::out | std::ios::binary);
    map_file << map.serialize();
    map_file.close();
}
