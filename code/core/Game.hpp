#pragma once

#include <SDL2/SDL.h>

#include "Input.hpp"
#include "Menu.hpp"
#include "Window.hpp"
#include "Level.hpp"
#include "Scene.hpp"

enum class GameState {
    START_MENU, LEVEL, QUIT
};

// Essentially a "Scene Manager". This is a top level class that reads user input and toggles between scenes.
class Game {
public:
    Game() : 
        state(GameState::START_MENU)
    {
        current_scene = &start_menu;
    }

    void tick() {
        for (InputEvent e = Input::get_event(); e != InputEvent::NONE; e = Input::get_event()) {
            switch (e) {
                case InputEvent::IGNORE:
                    break;
                case InputEvent::QUIT:
                    state = GameState::QUIT; 
                    break;
                case InputEvent::CONTINUE:
                    if (state == GameState::START_MENU) {
                        state = GameState::LEVEL;
                        current_scene = &level;
                        current_scene->enter();
                    } else if (state == GameState::LEVEL) {
                        state = GameState::START_MENU;
                        current_scene = &start_menu;
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
    // State
    Scene* current_scene;
    GameState state;
    // Scenes
    Menu start_menu;
    Level level;
};