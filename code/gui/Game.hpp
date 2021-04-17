#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include "Input.hpp"
#include "Menu.hpp"
#include "Window.hpp"
#include "Level.hpp"
#include "Scene.hpp"

enum class GameState {
    MENU, LEVEL, QUIT
};

class Game {
public:
    Game() : 
        state(GameState::MENU)
    {
        current_scene = &menu;
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
                        current_scene = &level;
                    }
                    break;
                default:
                    current_scene->handle_event(e);
            }
        }
        current_scene->tick();
        SDL_RenderClear(Window::renderer);
        current_scene->paint();
        SDL_RenderPresent(Window::renderer);
    }

    bool is_running() {
        return state != GameState::QUIT;
    }

private:
    Scene* current_scene;
    GameState state;
    Menu menu;
    Level level;
};