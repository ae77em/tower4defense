/* Implementacion basada en structs de atajos de teclado configurables

   Esta implementacion es configurable, pero no extensible en tiempo
   de ejecucion. Representa un compromiso entre keybindings basados en
   enums y keybindings basados en mapas en tiempo de ejecucion (eg, jsoncpp).
 */
#ifndef KEYBINDING_H
#define KEYBINDING_H

#include <SDL2/SDL.h>

struct Keybinding {
    SDL_Keycode up, down, left, right;
    SDL_Keycode water, earth, fire, air;
    SDL_Keycode portal_entry, portal_exit;
    SDL_Keycode road, firm_ground;
    SDL_Keycode horde;
    SDL_Keycode cancel;
};

extern const struct Keybinding default_keybinding;

#endif
