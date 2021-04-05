#include <SDL2/SDL.h>

#include <iostream>

#include "Window.hpp"

using namespace std;

int main() {
    Window::start();

    bool is_running = true;
    SDL_Event event;

    while (is_running) {
        auto before = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }
        SDL_SetRenderDrawColor(Window::renderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(Window::renderer);
        SDL_RenderPresent(Window::renderer);
        if (auto after = SDL_GetTicks(); after - before < 16) {
            SDL_Delay(after - before);
        }
    }

    Window::stop();

    return 0;
}
