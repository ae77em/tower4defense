#include "Screen.h"
#include "Keybinding.h"
#include "Map.h"
#include "Point.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

#ifndef EDITOR_H
#define EDITOR_H

namespace Editor {

class Editor;
class State;

class Editor {
    std::unique_ptr<State> state;
    Screen screen;
    model::Map map;
    Keybinding keys;
    bool scrolling_enabled;

    public:
    Editor(State *state);

    void run();

    void transition(State *newstate);
    void unsafe_transition(State *newstate);

    void load(std::string filename);
    void save(std::string filename);
    void new_map(unsigned side);

    /* Control map scrolling */
    void inhibitScrolling();
    void enableScrolling();

    Screen& getScreen();
    model::Map& getMap();
    const Keybinding& getKeys();
};

class State {
    public:
    virtual void handle(const SDL_Event &e, Editor &context) = 0;
    virtual void onTransition(Editor &context) {}
    virtual void preRender(Editor &context) {}
};

class StateTile : public State {
    char tile = 'x';

    public:
    virtual void handle(const SDL_Event &e, Editor &context);
};

class StatePath : public State {
    std::vector<Point> path;
    Point tentative_point = {0, 0};

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

/* Mostrar hordas, y agregar hordas nuevas */
class StateHordeManagement : public State {
    public:
    virtual void handle(const SDL_Event &e, Editor &context);
    virtual void onTransition(Editor &context);
};

/* Configurar nueva horda */
class StateHordeCreation : public State {
    int index;
    std::string enemy;
    int horde_size;
    int delay_seconds;

    public:
    explicit StateHordeCreation(int path_index);
    virtual void handle(const SDL_Event &e, Editor &context);
};

/* Solicitar datos al usuario y volver al estado anterior
 *
 * No transicionar a este estado normalmente. En su lugar utilizar una
 * unsafe_transition. El editor volvera al estado previo al terminar.
 *
 * El texto obtenido del usuario es usado como argumento al callback
 * proporcionado en el momento de la creacion del DataEntry. La forma
 * recomendada de utilizar este comportamiento es pasar al constructor
 * un lambda con referencias adecuadas a las variables internas del estado
 * original.
 */
class DataEntry : public State {
    State* previous_state;
    std::function<void (const std::string&)> callback;
    std::string prompt;
    std::string input;
    bool updated;

    public:
    DataEntry(State* previous_state,
            std::function<void (const std::string&)> callback,
            std::string prompt = "input: ");
    virtual void onTransition(Editor &context);
    virtual void handle(const SDL_Event &e, Editor &context);
    virtual void preRender(Editor &context);
};

} //namespace Editor

#endif
