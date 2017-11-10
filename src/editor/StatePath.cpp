#include "Editor.h"

void Editor::StatePath::handle(const SDL_Event &e, Editor &context) {
    // Simple, unshifted click adds points to path
    if (e.type == SDL_MOUSEBUTTONDOWN
            && !( KMOD_SHIFT & SDL_GetModState() )) {
        Point p = context.getScreen().mouseCurrentTile();
        Point d = context.getMap().dimensiones();
        if ((p.isPositive()) && (p.x < d.x) && (p.y < d.y))
            path.push_back(p);
    }

    // Shift-click adds point to path, ends it, and adds it to map
    if (e.type == SDL_MOUSEBUTTONDOWN
            && ( KMOD_SHIFT & SDL_GetModState() )) {
        Point p = context.getScreen().mouseCurrentTile();
        Point d = context.getMap().dimensiones();

        /* Do something iff the new point is acceptable as
           an extension of the path, and the result would be
           a sensible path */
        if ((p.isPositive()) && (p.x < d.x) && (p.y < d.y)
                && (path.size() > 0)) {
            path.push_back(p);
            context.getMap().agregarCamino(path);
            path.clear();
        }
    }

    const Keybinding& keys = context.getKeys();

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.cancel) {
        path.clear();
    }

    // Change mode
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == keys.road) {
        context.transition(new StateTile());
    }
}

void Editor::StatePath::onTransition(Editor &context) {
    context.getScreen().setDialog("-- PATH --");
}
