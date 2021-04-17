#pragma once

#include <SDL2/SDL.h>

/*********************************************
 *              KEYBOARD STATE
 *********************************************/

// The input state is updated when SDL_PollEvent() is called.
// This is fine because we handle events before calling tick() in Game::tick().
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
    LEFT,
    RIGHT,
    ATTACK,
    JUMP,
    IGNORE,  // an SDL InputEvent that we don't care about
};

// Get the next SDL InputEvent and convert it into an in-game InputEvent.
InputEvent get_event();