#pragma once

#include <SDL2/SDL.h>

// The different event types in this game.
enum Event {
    NONE = 0,  // no events in the SDL queue
    CONTINUE,
    QUIT,
    IGNORE  // an SDL event that we don't care about
};

// Get the next SDL event and convert it into an in-game event.
Event get_event() {
    SDL_Event e;
    if (!SDL_PollEvent(&e)) {
        return NONE;
    }
    
    if (e.type == SDL_QUIT) {
        return QUIT;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                return QUIT;
            case SDLK_RETURN:
                return CONTINUE;
        }
    } else {
        return IGNORE;
    }
}