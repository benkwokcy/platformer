#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#include "Window.hpp"
#include "Menu.hpp"
#include "Event.hpp"

using namespace std;

int main() {
    Window::start();

    auto scene = shared_ptr<Scene>(new Menu());
    bool is_running = true;

    while (is_running) {
        auto before = SDL_GetTicks();
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            switch (Event game_event = translate_sdl_event(sdl_event)) {
                case QUIT: is_running = false; break;
                default: scene->handle_event(game_event);
            }
        }
        scene->tick();
        scene->paint();
        SDL_RenderPresent(Window::renderer);
        if (auto after = SDL_GetTicks(); after - before < 16) {
            SDL_Delay(after - before);
        }
    }

    Window::stop();

    return 0;
}
