#include <SDL2/SDL.h>

#include "Event.hpp"

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
            case SDLK_LEFT:
                return Event::LEFT_PRESS;
            case SDLK_RIGHT:
                return Event::RIGHT_PRESS;
            case SDLK_SPACE:
                return Event::ATTACK;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                return Event::LEFT_RELEASE;
            case SDLK_RIGHT:
                return Event::RIGHT_RELEASE;
        }
    }

    return Event::IGNORE;
}