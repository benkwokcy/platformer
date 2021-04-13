#pragma once

#include <SDL2/SDL.h>

/*********************************************
 *              KEYBOARD STATE
 *********************************************/

// The input state is updated when SDL_PollEvent() is called.
// This is fine because we handle events before calling tick() in Game::do_one_frame().
class Input {
public:
    static inline const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    static bool left_down() {
        return keyboard_state[SDL_GetScancodeFromKey(SDLK_LEFT)];
    }

    static bool right_down() {
        return keyboard_state[SDL_GetScancodeFromKey(SDLK_RIGHT)];
    }
};

/*********************************************
 *              KEYBOARD EVENTS
 *********************************************/

enum class InputEvent {
    NONE,  // no events in the SDL queue
    CONTINUE,
    QUIT,
    LEFT_PRESS,
    LEFT_RELEASE,
    RIGHT_PRESS,
    RIGHT_RELEASE,
    ATTACK,
    JUMP,
    IGNORE,  // an SDL InputEvent that we don't care about
};

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
                return InputEvent::LEFT_PRESS;
            case SDLK_RIGHT:
                return InputEvent::RIGHT_PRESS;
            case SDLK_SPACE:
                return InputEvent::ATTACK;
            case SDLK_UP:
                return InputEvent::JUMP;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                return InputEvent::LEFT_RELEASE;
            case SDLK_RIGHT:
                return InputEvent::RIGHT_RELEASE;
        }
    }

    return InputEvent::IGNORE;
}