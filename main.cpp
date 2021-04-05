#include <SDL2/SDL.h>

#include <iostream>

#include "Window.hpp"

using namespace std;

int main() {
    Window::start();

    // Main Loop
    bool is_running = true;
    SDL_Event event;

    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }
        SDL_SetRenderDrawColor(Window::renderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(Window::renderer);
        SDL_RenderPresent(Window::renderer);
        SDL_Delay(16);
    }

    Window::stop();

    return 0;
}
