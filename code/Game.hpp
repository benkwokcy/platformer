#pragma once

#include <SDL2/SDL.h>

#include <memory>

#include "Event.hpp"
#include "Menu.hpp"
#include "Window.hpp"

using namespace std;

class Game {
public:
    bool is_running;

    Game() {
        scene = make_unique<Menu>();
        is_running = true;
    }

    void do_one_frame() {
        while (Event e = get_event()) {
            switch (e) {
                case QUIT:
                    is_running = false; break;
                default:
                    scene->handle_event(e);
            }
        }
        scene->tick();
        scene->paint();
        SDL_RenderPresent(Window::renderer);
    }

private:
    unique_ptr<Entity> scene;
};