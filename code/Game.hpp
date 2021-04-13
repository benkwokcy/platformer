#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include "Input.hpp"
#include "Menu.hpp"
#include "Window.hpp"
#include "Level.hpp"

enum class GameState {
    MENU, LEVEL, QUIT
};

// Switches the game between different scenes.
class Game {
public:
    Game() : 
        state(GameState::MENU)
    {
        scene = &menu;
    }

    void tick() {
        for (InputEvent e = get_event(); e != InputEvent::NONE; e = get_event()) {
            switch (e) {
                case InputEvent::IGNORE:
                    break;
                case InputEvent::QUIT:
                    state = GameState::QUIT; 
                    break;
                case InputEvent::CONTINUE:
                    if (state == GameState::MENU) {
                        state = GameState::LEVEL;
                        scene = &level;
                    }
                    break;
                default:
                    scene->handle_event(e);
            }
        }
        scene->tick();
        SDL_RenderClear(Window::renderer);
        scene->paint();
        SDL_RenderPresent(Window::renderer);
    }

    bool is_running() {
        return state != GameState::QUIT;
    }

private:
    Entity* scene;
    GameState state;
    Menu menu;
    Level level;
};