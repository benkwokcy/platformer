#include <SDL2/SDL.h>

#include "Input.hpp"

// Get the next SDL InputEvent and convert it into an in-game InputEvent.
InputEvent get_event() {
    SDL_Event e;
    if (!SDL_PollEvent(&e)) { 
        return InputEvent::NONE; 
    }
    
    if (e.type == SDL_QUIT) {
        return InputEvent::QUIT;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                return InputEvent::QUIT;
            case SDLK_RETURN:
                return InputEvent::CONTINUE;
            case SDLK_LEFT:
                return InputEvent::LEFT;
            case SDLK_RIGHT:
                return InputEvent::RIGHT;
            case SDLK_SPACE:
                return InputEvent::ATTACK;
            case SDLK_UP:
                return InputEvent::JUMP;
        }
    }

    return InputEvent::IGNORE;
}