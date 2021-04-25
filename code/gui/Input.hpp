#pragma once

#include <SDL2/SDL.h>

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

/*********************************************
 *              KEYBOARD STATE
 *********************************************/

class Input {
public:
    // This keyboard_state is silently updated when SDL_PollEvent() is called.
    // So get_event() must be called before calling is_left_down() or is_right_down().
    // We could update the keyboard_state manually in is_left_down() or is_right_down() but 
    // that might cause our keyboard state to update to the next frame while we are halfway through the current one.
    static inline const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    static bool is_left_down() {
        return keyboard_state[SDL_GetScancodeFromKey(SDLK_LEFT)];
    }

    static bool is_right_down() {
        return keyboard_state[SDL_GetScancodeFromKey(SDLK_RIGHT)];
    }

    // Get the next SDL InputEvent and convert it into an in-game InputEvent.
    static InputEvent get_event() {
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
};