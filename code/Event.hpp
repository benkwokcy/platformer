#pragma once

#include <SDL2/SDL.h>

// The different event types in this game.
enum Event {
    QUIT,
    NONE
};

// Map an SDL event to an internal game event.
Event translate_sdl_event(const SDL_Event e) {
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
        return QUIT;
    } else {
        return NONE;
    }
}