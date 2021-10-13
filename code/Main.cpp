#include <SDL2/SDL.h>

#include "Window.hpp"
#include "Game.hpp"
#include "util/PerformanceMonitor.hpp"

int main() {
    Window::start();
    PerformanceMonitor monitor;
    Game game;

    while (game.is_running()) {
        auto before = SDL_GetTicks();
        game.tick();
        auto after = SDL_GetTicks();
        if (auto ms = after - before; ms < 16) {
            SDL_Delay(16 - ms);
        }
        monitor.update(after - before);
    }

    Window::stop();
    return 0;
}