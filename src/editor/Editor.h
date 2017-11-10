#include "../sdl/Screen.h"
#include "../sdl/Keybinding.h"
#include "../common/modelo/Mapa.h"
#include "../common/Point.h"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

#ifndef EDITOR_H
#define EDITOR_H

namespace Editor {

class Editor;
class State;
class StateTile;
class StatePath;

class Editor {
    std::unique_ptr<State> state;
    Screen screen;
    Mapa map;
    Keybinding keys;

    public:
    Editor(State *state);

    void run();
    void transition(State *newstate);

    Screen& getScreen();
    Mapa& getMap();
    const Keybinding& getKeys();
};

class State {
    public:
    virtual void handle(const SDL_Event &e, Editor &context) = 0;
};

class StateTile : public State {
    char tile = '~';

    public:
    virtual void handle(const SDL_Event &e, Editor &context);
};

class StatePath : public State {
    std::vector<Point> path;

    public:
    virtual void handle(const SDL_Event &e, Editor &context);
};

} //namespace Editor

#endif
