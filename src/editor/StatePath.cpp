#include "Editor.h"

void Editor::StatePath::handle(const SDL_Event &e, Editor &context) {
    auto& map = context.getMap();
    auto& screen = context.getScreen();

    // Simple, unshifted click adds points to path
    if (e.type == SDL_MOUSEBUTTONDOWN
            && !( KMOD_SHIFT & SDL_GetModState() )) {
        Point p = screen.mouseCurrentTile();
        if (map.estaDentro(p)) {
            // Automatically add portal on the first point
            if (path.size() == 0) map.setCasilla('E', p.x, p.y);

            path.push_back(p);
        }
    }

    // Shift-click adds point to path, ends it, and adds it to map
    if (e.type == SDL_MOUSEBUTTONDOWN
            && ( KMOD_SHIFT & SDL_GetModState() )) {
        Point p = screen.mouseCurrentTile();

        /* Do something iff the new point is acceptable as
           an extension of the path, and the result would be
           a sensible path */
        if (map.estaDentro(p) && (path.size() > 0)) {
            // Automatically add portal on the last point
            map.setCasilla('S', p.x, p.y);
            path.push_back(p);
            map.agregarCamino(path);
            path.clear();
        }
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
    context.getScreen().trace(path);
}
