#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <filesystem>

#include "Window.hpp"
#include "Menu.hpp"

using namespace std;

int main() {
    Window::start();

    Scene* scene = new Menu();

    bool is_running = true;
    SDL_Event event;

    while (is_running) {
        auto before = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                is_running = false;
            }
        }
        scene->paint();
        SDL_RenderPresent(Window::renderer);
        if (auto after = SDL_GetTicks(); after - before < 16) {
            SDL_Delay(after - before);
        }
    }

    Window::stop();

    return 0;
}
