#pragma once

#include <SDL2/SDL.h>

// The different event types in this game.
enum Event {
    QUIT,
    IGNORE, // event is ignored
    NONE    // no events in the queue
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