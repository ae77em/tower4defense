#include "../sdl/Screen.h"
#include "../sdl/Keybinding.h"
#include "../common/modelo/Mapa.h"
#include "../common/Point.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

#ifndef EDITOR_H
#define EDITOR_H

namespace Editor {

class Editor;
class State;

class Editor {
    std::unique_ptr<State> state;
    Screen screen;
    model::Mapa map;
    Keybinding keys;
    bool scrolling_enabled;

    public:
    Editor(State *state);

    void run();
    void transition(State *newstate);
    void load(std::string filename);
    void save(std::string filename);
    void new_map(unsigned side);

    /* Control map scrolling */
    void inhibitScrolling();
    void enableScrolling();

    Screen& getScreen();
    model::Mapa& getMap();
    const Keybinding& getKeys();
};

class State {
    public:
    virtual void handle(const SDL_Event &e, Editor &context) = 0;
    virtual void onTransition(Editor &context) {}
    virtual void preRender(Editor &context) {}
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
    virtual void onTransition(Editor &context);
    virtual void preRender(Editor &context);
};

typedef std::string (*command_t)(Editor&, const std::string&);
class StateCommand : public State {
    std::string command;
    std::map<std::string, command_t> dispatch_table;

    public:
    StateCommand();
    virtual void handle(const SDL_Event &e, Editor &context);
    virtual void onTransition(Editor &context);
};

} //namespace Editor

#endif
