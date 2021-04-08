#pragma once

#include <SDL2/SDL.h>

// The different event types in this game.
enum class Event {
    NONE,  // no events in the SDL queue
    CONTINUE,
    QUIT,
    IGNORE  // an SDL event that we don't care about
};

// Get the next SDL event and convert it into an in-game event.
Event get_event() {
    SDL_Event e;
    
    if (!SDL_PollEvent(&e)) {
        return Event::NONE;
    }
    
    if (e.type == SDL_QUIT) {
        return Event::QUIT;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                return Event::QUIT;
            case SDLK_RETURN:
                return Event::CONTINUE;
        }
    }

    return Event::IGNORE;
}