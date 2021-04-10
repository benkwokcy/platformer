#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include "Event.hpp"
#include "Menu.hpp"
#include "Window.hpp"
#include "Level.hpp"

using namespace std;

enum class GameState {
    MENU, LEVEL, QUIT
};

// Switches the game between different scenes.
class Game {
public:
    Game() : state(GameState::MENU)
    {
        scene = &menu;
    }

    void do_one_frame() {
        for (Event e = get_event(); e != Event::NONE; e = get_event()) {
            switch (e) {
                case Event::IGNORE:
                    break;
                case Event::QUIT:
                    state = GameState::QUIT; 
                    break;
                case Event::CONTINUE:
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