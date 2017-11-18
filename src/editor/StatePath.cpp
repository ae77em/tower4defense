#include "Editor.h"
#include "../sdl/Utils.h"

void Editor::StatePath::handle(const SDL_Event &e, Editor &context) {
    auto& map = context.getMap();
    auto& screen = context.getScreen();

    // Simple, unshifted click adds points to path
    if (e.type == SDL_MOUSEBUTTONDOWN
            && !( KMOD_SHIFT & SDL_GetModState() )) {
        // The first point is taken from current mouse position
        if (path.size() == 0) {
            Point p = screen.mouseCurrentTile();
            if (! map.estaDentro(p)) return;
            map.setCasilla('E', p.x, p.y);
            path.push_back(p);
        // Points after the first are taken from tentative_point
        } else {
            if (! map.estaDentro(tentative_point)) return;
            path.push_back(tentative_point);
        }
    }

    // Shift-click adds point to path, ends it, and adds it to map
    if (e.type == SDL_MOUSEBUTTONDOWN
            && ( KMOD_SHIFT & SDL_GetModState() )) {
        Point p = tentative_point;

        /* Do something iff the new point is acceptable as
           an extension of the path, and the result would be
           a sensible path */
        if (! (map.estaDentro(p) && (path.size() > 0)) ) return;

        // Automatically add portal on the last point
        map.setCasilla('S', p.x, p.y);
        path.push_back(p);
        map.agregarCamino(path);
        path.clear();
    }

    const Keybinding& keys = context.getKeys();

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.cancel) {
        path.clear();
    }

    // Change mode
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.road) {
        screen.setDialog("");
        context.transition(new StateTile());
    }
}

void Editor::StatePath::onTransition(Editor &context) {
    context.getScreen().setDialog("-- PATH --");
}

void Editor::StatePath::preRender(Editor &context) {
    auto& screen = context.getScreen();
    screen.trace(path);

    if (path.size() == 0) return;
    const auto c = screen.mouseCurrentTile();
    const auto& p = path.back();

    /* Select the closest point to c from the points adjacent
       to the last point in path */
    const std::vector<Point> surroundings = {
        {p.x - 1, p.y - 1}, {p.x, p.y - 1}, {p.x + 1, p.y - 1},
        {p.x - 1, p.y    },                 {p.x + 1, p.y    },
        {p.x - 1, p.y + 1}, {p.x, p.y + 1}, {p.x + 1, p.y + 1}
    };
    tentative_point = Utils::findClosest(c, surroundings);

    /* Show the new tentative segment */
    screen.trace({p, tentative_point});
}
