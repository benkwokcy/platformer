#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include "Event.hpp"
#include "Menu.hpp"
#include "Window.hpp"
#include "Level.hpp"

using namespace std;

enum GameState {
    SHOWING_MENU, PLAYING
};

// Switches the game between different scenes.
class Game {
public:
    bool is_running;

    Game() :
        state(SHOWING_MENU),
        is_running(true)
    {
        scene = &menu;
    }

    void do_one_frame() {
        while (Event e = get_event()) {
            switch (e) {
                case QUIT:
                    is_running = false; break;
                case CONTINUE:
                    if (state == SHOWING_MENU) {
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

private:
    Entity* scene;
    GameState state;
    Menu menu;
    Level level;
};