#pragma once

#include <SDL2/SDL.h>

// The different event types in this game.
enum Event {
    NONE = 0,  // no events in the SDL queue
    QUIT,
    IGNORE  // an SDL event that we don't care about
};

// Get the next SDL event and convert it into an in-game event.
Event get_event() {
    SDL_Event e;
    if (!SDL_PollEvent(&e)) {
        return NONE;
    }
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
        return QUIT;
    } else {
        return IGNORE;
    }
}